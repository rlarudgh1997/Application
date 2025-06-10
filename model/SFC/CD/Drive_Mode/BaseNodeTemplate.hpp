/**
 * @file BaseNodeTemplate.hpp
 * @author Infotainment Software Development Team
 *
 * Copyright (c) 2021  Hyundai Motor Company,
 * All Rights Reserved.
 *
 * Use and copying of this software and preparation of derivative works
 * based upon this software are permitted. Any copy of this software or
 * of any derivative work must include the above copyright notice, this
 * paragraph and the one after it. Any distribution of this software or
 * derivative works must comply with all applicable laws.
 *
 * This software is made available AS IS, and COPYRIGHT OWNERS DISCLAIMS
 * ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE, AND NOTWITHSTANDING ANY OTHER PROVISION CONTAINED HEREIN, ANY
 * LIABILITY FOR DAMAGES RESULTING FROM THE SOFTWARE OR ITS USE IS
 * EXPRESSLY DISCLAIMED, WHETHER ARISING IN CONTRACT, TORT (INCLUDING
 * NEGLIGENCE) OR STRICT LIABILITY, EVEN IF COPYRIGHT OWNERS ARE ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGES.
 */

#ifndef SSFS_Drive_Mode_BaseNodeTemplate_H
#define SSFS_Drive_Mode_BaseNodeTemplate_H

#include <functional>

namespace ccos {
class EventBase {
public:
    EventBase() = default;
    virtual ~EventBase() = default;

public:
    virtual std::string process() = 0;
};

template <typename T>
class BaseNodeProp {
public:
    explicit BaseNodeProp(const T& nodeValue, const std::function<void()>& _callback) : mValue(nodeValue), mCallback(_callback) {
    }
    BaseNodeProp& operator=(const BaseNodeProp& other) = delete;
    BaseNodeProp& operator=(BaseNodeProp&& other) noexcept = delete;

    operator T() {
        return mValue;
    }

    T operator=(const T& nodeValue) {
        if (mValue != nodeValue) {
            mValue = nodeValue;

            if (mCallback != nullptr) {
                mCallback();
            }
        }
        return mValue;
    }

    T selfValue() {
        return mValue;
    }

private:
    T mValue;
    std::function<void()> mCallback;
};

template <typename T>
class BaseNode {
public:
    explicit BaseNode(const T& nodeValue, const bool _initialFlush = false) : mValue(nodeValue), mIsNeedFlushed(_initialFlush) {
    }
    virtual ~BaseNode() = default;

    operator T() {
        return mValue;
    }

    T operator=(const T& nodeValue) {
        if (mValue != nodeValue) {
            mValue = nodeValue;
        }
        return mValue;
    }

    void renew() {
        if (mIsNeedUpdated == true) {
            T value = process();
            if (mValue != value) {
                mValue = value;
                mIsNeedFlushed = true;
            }
            mIsNeedUpdated = false;
        }
    }

    void setValue(const T& nodeValue) {
        if (nodeValue > T::NONE && nodeValue < T::MAX) {
            mValue = nodeValue;
        }
    }

    bool isNeedUpdated() {
        return mIsNeedUpdated;
    }

    bool isNeedFlushed() {
        return mIsNeedFlushed;
    }

    void setNeedFlushed(bool _needFlushed) {
        mIsNeedFlushed = _needFlushed;
    }

    void setCallback(std::function<void()> _callback) {
        mCallback = _callback;
    }

    T selfValue() {
        return mValue;
    }

    virtual T process() = 0;

private:
    T mValue;

protected:
    bool mIsNeedUpdated = false;

    void setNeedUpdate() {
        mIsNeedUpdated = true;
    }

    bool mIsNeedFlushed;
    std::function<void()> mCallback;
};
}  // namespace ccos
#endif  // DRIVE_MODE_R000_BASENODETEMPLATE_HPP
