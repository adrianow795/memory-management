#ifndef SHARED_PRT_H_
#define SHARED_PRT_H_

#include <atomic>
#include <functional>

namespace my {

template <class T>
struct ControlBlock {
    std::atomic<size_t> shared_refs;
    std::atomic<size_t> weak_refs;
    std::function<void(T*)> deleter;
};

template <class Type1>
auto default_deleter = [](Type1* p) { delete p; };

template <class Type>
class shared_ptr {
private:
    Type* ptr_;
    ControlBlock<Type>* ctrl_block_;

public:
    shared_ptr(Type* ptr, std::function<void(Type*)> d = default_deleter<Type>)
        : ptr_(ptr), ctrl_block_(new ControlBlock<Type>{0, 0, d}) {
        // ctrl_block_ = new ControlBlock<Type>{0,0,nullptr};
        ctrl_block_->shared_refs++;
    }

    shared_ptr(const shared_ptr& other)
        : ptr_(other.ptr_), ctrl_block_(other.ctrl_block_) {
        ctrl_block_->shared_refs++;
    }

    shared_ptr(shared_ptr&& other)
        : ptr_(other.ptr_), ctrl_block_(other.ctrl_block_) {
        other.ptr_ = nullptr;
        other.ctrl_block_ = nullptr;
    }

    ~shared_ptr() {
        if (ctrl_block_) {
            ctrl_block_->shared_refs--;
            if (ctrl_block_->shared_refs == 0) {
                ctrl_block_->deleter(ptr_);

                if (ctrl_block_->weak_refs == 0) {
                    delete ctrl_block_;
                }
            }
        }
    }

    Type* operator->() {
        return ptr_;
    }

    Type& operator*() {
        return *ptr_;
    }

    explicit operator bool() const {
        return (ptr_ != nullptr);
    }

    size_t use_count() {
        return ctrl_block_->shared_refs;
    }

    Type* get() {
        return ptr_;
    }

    void reset(Type* ptr = nullptr, std::function<void(Type*)> d = default_deleter<Type>)
    {
        if(ptr_ && ctrl_block_->shared_refs > 0)
        {
            ctrl_block_->deleter(ptr_);
            ptr_ = ptr;
            ctrl_block_->deleter = d;
        }
        else if(!ptr_)
        {
            if(ctrl_block_)
            {
                ptr_ = ptr;
                ctrl_block_->shared_refs++;
            }
            else
            {
                ptr_ = ptr;
                ctrl_block_ = new ControlBlock<Type>{0, 0, d};
                ctrl_block_->shared_refs++;
            }

        }

    }
};

}  // namespace my
#endif /* SHARED_PRT_H_*/

