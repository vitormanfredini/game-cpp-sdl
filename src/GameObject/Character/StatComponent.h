#pragma once
#include <memory>

class StatComponent {
private:
    float initialValue;
    float value;
public:
    StatComponent(float initialValue): value(initialValue), initialValue(initialValue) {
        //
    }
    StatComponent(float initialValue, float value): initialValue(initialValue), value(value) {
        //
    }
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
        return std::make_unique<StatComponent>(initialValue, value);
    };
};