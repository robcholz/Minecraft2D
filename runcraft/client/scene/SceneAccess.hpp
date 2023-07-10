//
// Created by robcholz on 4/29/23.
//

#ifndef RUNCRAFT_SCENEACCESS_HPP
#define RUNCRAFT_SCENEACCESS_HPP

class SceneAccess {
protected:
	void runScene(){
		state=State::RUNNING;
	}

	void pauseScene(){
		state=State::PAUSED;
	}

	void terminateScene(){
		state=State::TERMINATED;
	}
public:
	enum class State{RUNNING,PAUSED,TERMINATED};
	bool isRunning() {
		return (state==State::RUNNING);
	}

	bool isPaused() {
		return (state==State::PAUSED);
	}

	bool isTerminated() {
		return (state==State::TERMINATED);
	}

	virtual void onUpdate()=0;
	virtual void onRender()=0;

	virtual ~SceneAccess()=default;
private:
	State state{};
};

#endif //RUNCRAFT_SCENEACCESS_HPP
