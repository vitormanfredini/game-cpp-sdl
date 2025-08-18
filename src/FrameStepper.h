#pragma once

class FrameStepper {
public:
    FrameStepper(int totalFrames, int updatesPerFrame): totalFrames(totalFrames), updatesPerFrame(updatesPerFrame) {
        //
    }

    void update(){
        currentUpdate += 1;
        if(currentUpdate >= updatesPerFrame){
            currentUpdate = 0;
            currentFrame += 1;
            if(currentFrame >= totalFrames){
                currentFrame = 0;
            }
        }
    }

    int getFrame(){
        return currentFrame;
    }

    void reset(){
        currentUpdate = -1;
        currentFrame = 0;
    }

private:
    int totalFrames;
    int updatesPerFrame;
    int currentUpdate = -1;
    int currentFrame = 0;
};
