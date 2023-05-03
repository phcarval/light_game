#ifndef WINDOW_CONTEXT_H
#define WINDOW_CONTEXT_H

class InputStatus;

class WindowContext
{
public:
	virtual WindowContext* updateContext(const InputStatus& status) = 0;
	virtual void render() = 0;

	virtual ~WindowContext() {}
};

#endif //WINDOW_CONTEXT_H