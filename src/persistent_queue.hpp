#include <bits/stdc++.h>

// immutable stack interface
template <typename T>
struct PersistentStack {
  virtual ~PersistentStack() = default;

  virtual std::shared_ptr<PersistentStack<T>> push(T item) const = 0;

  virtual std::shared_ptr<PersistentStack<T>> pop() const = 0;

  virtual std::optional<T> top() const = 0;

  virtual int size() const = 0;

  bool empty() const { return size() == 0; }

 private:
  mutable std::optional<std::shared_ptr<PersistentStack<T>>> rev_;

  template <typename U>
  friend std::shared_ptr<PersistentStack<U>> reverse_stack(
      std::shared_ptr<PersistentStack<U>> st);
};

template <typename T>
struct LinkedList : public PersistentStack<T> {
  struct Node;
  using NodePtr = std::shared_ptr<Node>;

  struct Node {
    T val;
    NodePtr next;
    Node(T val, NodePtr next) : val(std::move(val)), next(std::move(next)) {}
  };

  LinkedList() : node_{}, size_(0) {}
  ~LinkedList() override = default;

  static const std::shared_ptr<LinkedList<T>> &nil() {
    static const std::shared_ptr<LinkedList<T>> kNil =
        std::make_shared<LinkedList<T>>();
    return kNil;
  }

  std::shared_ptr<PersistentStack<T>> push(T item) const override {
    return std::shared_ptr<PersistentStack<T>>{new LinkedList<T>(
        std::make_shared<Node>(std::move(item), node_), size_ + 1)};
  }

  std::shared_ptr<PersistentStack<T>> pop() const override {
    assert(node_ != nullptr);
    return std::shared_ptr<PersistentStack<T>>{
        new LinkedList<T>(node_->next, size_ - 1)};
  }

  std::optional<T> top() const override {
    if (node_ == nullptr) return std::nullopt;
    return node_->val;
  }

  int size() const override { return size_; }

 private:
  NodePtr node_;
  int size_;

  LinkedList(NodePtr node, int size) : node_(std::move(node)), size_(size) {}
};

template <typename T>
struct ReversedStack : public PersistentStack<T> {
  explicit ReversedStack(std::shared_ptr<PersistentStack<T>> st)
      : wrapped_(std::move(st)) {}
  ~ReversedStack() override = default;

  std::shared_ptr<PersistentStack<T>> push(T item) const override {
    return evaluate()->push(std::move(item));
  }

  std::shared_ptr<PersistentStack<T>> pop() const override {
    return evaluate()->pop();
  }

  std::optional<T> top() const override { return evaluate()->top(); }

  int size() const override { return wrapped_->size(); }

 private:
  std::shared_ptr<PersistentStack<T>> wrapped_;
  mutable std::optional<std::shared_ptr<PersistentStack<T>>> evaluated_;

  std::shared_ptr<PersistentStack<T>> evaluate() const {
    if (not evaluated_) {
      std::shared_ptr<PersistentStack<T>> rev =
          std::make_shared<LinkedList<T>>();
      for (std::shared_ptr<PersistentStack<T>> p = wrapped_; not p->empty();
           p = p->pop()) {
        rev = rev->push(*p->top());
      }
      evaluated_ = std::move(rev);
    }
    return *evaluated_;
  }
};

template <typename T>
std::shared_ptr<PersistentStack<T>> reverse_stack(
    std::shared_ptr<PersistentStack<T>> st) {
  if (not st->rev_) {
    st->rev_ = std::make_shared<ReversedStack<T>>(st);
  }
  return *(st->rev_);
}

template <typename T>
struct ConcatenatedStack : public PersistentStack<T> {
  explicit ConcatenatedStack(std::shared_ptr<PersistentStack<T>> st1,
                             std::shared_ptr<PersistentStack<T>> st2)
      : stack1_(std::move(st1)), stack2_(std::move(st2)) {}
  ~ConcatenatedStack() override = default;

  std::shared_ptr<PersistentStack<T>> push(T item) const override {
    return std::make_shared<ConcatenatedStack<T>>(stack1_, stack2_->push(item));
  }

  std::shared_ptr<PersistentStack<T>> pop() const override {
    if (stack1_->empty()) return stack2_->pop();
    auto st1 = stack1_->pop();
    if (st1->empty()) return stack2_;
    return std::make_shared<ConcatenatedStack<T>>(std::move(st1), stack2_);
  }

  std::optional<T> top() const override {
    if (stack1_->empty()) return stack2_->top();
    return stack1_->top();
  }

  int size() const override { return stack1_->size() + stack2_->size(); }

 private:
  std::shared_ptr<PersistentStack<T>> stack1_, stack2_;
};

// Banker's Queue
template <typename T>
struct PersistentQueue {
  mutable std::shared_ptr<PersistentStack<T>> front_, rear_;

  PersistentQueue()
      : front_(LinkedList<T>::nil()), rear_(LinkedList<T>::nil()) {}

  PersistentQueue push(T item) const {
    auto r = rear_->push(item);
    if (r->size() > front_->size() + 1) {
      auto f = std::make_shared<ConcatenatedStack<T>>(
          front_, reverse_stack(std::move(r)));
      return PersistentQueue(std::move(f), LinkedList<T>::nil());
    }
    return PersistentQueue(front_, std::move(r));
  }

  PersistentQueue pop() const {
    if (not front_->empty()) {
      return PersistentQueue(front_->pop(), rear_);
    }
    assert(not rear_->empty());
    return PersistentQueue(reverse_stack(rear_), LinkedList<T>::nil());
  }

  std::optional<T> top() const {
    if (empty()) return std::nullopt;
    if (front_->empty()) {
      front_ = reverse_stack(rear_);
      rear_ = LinkedList<T>::nil();
    }
    return front_->top();
  }

  bool empty() const { return front_->empty() and rear_->empty(); }

  int size() const { return front_->size() + rear_->size(); }

 private:
  PersistentQueue(std::shared_ptr<PersistentStack<T>> f,
                  std::shared_ptr<PersistentStack<T>> r)
      : front_(std::move(f)), rear_(std::move(r)) {}
};
