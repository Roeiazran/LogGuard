#pragma once

#include <utility>
#include <variant>

namespace logguard {

// Marks a value as an error when constructing a Result: `return Err{ParseError::kEmptyLine};`
template <typename E>
struct Err {
    E error;
};

template <typename E>
Err(E) -> Err<E>;

// Holds either a value of type T (success) or an error of type E (failure).
// Expected, data-driven failures are returned as values instead of thrown (ADR-0007).
// Same idea as C++23 std::expected; we stay on C++20.
template <typename T, typename E>
class [[nodiscard]] Result {
public:
    explicit(false) Result(T value) : data_(std::in_place_index<0>, std::move(value)) {}
    explicit(false) Result(Err<E> err) : data_(std::in_place_index<1>, std::move(err.error)) {}

    [[nodiscard]] bool HasValue() const noexcept { return data_.index() == 0; }
    explicit operator bool() const noexcept { return HasValue(); }

    // Calling Value() on an error (or Error() on a value) is a programming bug:
    // it throws std::bad_variant_access.
    [[nodiscard]] T& Value() & { return std::get<0>(data_); }
    [[nodiscard]] const T& Value() const& { return std::get<0>(data_); }
    [[nodiscard]] T&& Value() && { return std::get<0>(std::move(data_)); }

    [[nodiscard]] const E& Error() const& { return std::get<1>(data_); }

private:
    std::variant<T, E> data_;
};

}  // namespace logguard
