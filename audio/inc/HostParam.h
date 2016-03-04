#pragma once

#include <atomic>

#include "JuceHeader.h"
#include "Param.h"

template<typename _par>
class HostParam : public AudioProcessorParameter, public Param::Listener {
public:
    HostParam(_par &p) : param(p) {
        param.addListener(this);
    }

    ~HostParam() {
        param.removeListener(this);
    }

    float getValue() const override {
        return engineToHost(param.getUI());
    }

    void setValue(float newValue) override {
        jassert(newValue >= 0.f && newValue <= 1.f);
        param.setHost(hostToEngine(newValue));
    }

    virtual float getDefaultValue() const override {
        return engineToHost(param.getDefault());
    }

    virtual String getName(int maximumStringLength) const override {
        return param.hostTag().substring(0,maximumStringLength);
    }

    virtual String getLabel() const override {
        return param.unit();
    }

    virtual int getNumSteps() const override {
        return param.getNumSteps();
    }

    //! \todo implement this
    virtual float getValueForText(const String& text) const override {
        return text.getFloatValue();
    }

    virtual String getText (float value, int maximumStringLength) const override {
        return param.getUIString(hostToEngine(value)).substring(0, maximumStringLength);
    }

    virtual void paramUIChanged() override {
        // this will (unnecessarily) call setValue
        setValueNotifyingHost(engineToHost(param.getUI()));
    }

protected:
    virtual float engineToHost(float engineVal) const {
        jassert(engineVal >= param.getMin() && engineVal <= param.getMax());
        return (engineVal - param.getMin()) / (param.getMax() - param.getMin());
    }
    virtual float hostToEngine(float hostVal) const {
        jassert(hostVal >= 0.f && hostVal <= 1.f);
        if(param.getNumSteps()==0) {
            return (param.getMin() + hostVal*(param.getMax() - param.getMin()));
        } else {
            return trunc(hostVal*static_cast<float>(param.getNumSteps()-1) + .5f);
        }
    }

    _par &param;
};

template<typename _par>
class HostParamLog : public HostParam<_par> {
public:
    using HostParam<_par>::HostParam;

protected:
    float engineToHost(float engineVal) const override {
        jassert(engineVal >= param.getMin() && engineVal <= param.getMax());
        return log(engineVal - param.getMin() + 1e-1f) / log(param.getMax() - param.getMin() + 1e-1f);
    }
    float hostToEngine(float hostVal) const override {
        jassert(hostVal >= 0.f && hostVal <= 1.f);
        jassert(param.getNumSteps() == 0);

        return param.getMin() + exp(hostVal * log(param.getMax() - param.getMin() + 1e-1f)) - 1e-1f;
    }
};