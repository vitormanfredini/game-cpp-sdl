#pragma once
#include <memory>

class StatComponent {
private:
    float initialValue;
    float value;
public:
    StatComponent(float initialValue): initialValue(initialValue), value(initialValue) {}
    virtual float getInitialValue(){
        return initialValue;
    };
    virtual float getValue(){
        return value;
    };
    virtual void setValue(float newValue){
        value = newValue;
    };
    virtual void setInitialValue(float newInitialValue){
        initialValue = newInitialValue;
    };
    virtual void add(float amount){
        value += amount;
    };
    virtual ~StatComponent() = default;
    virtual std::unique_ptr<StatComponent> clone() {
        std::unique_ptr<StatComponent> copy = std::make_unique<StatComponent>(initialValue);
        copy->setValue(value);
        return copy;
    };
};