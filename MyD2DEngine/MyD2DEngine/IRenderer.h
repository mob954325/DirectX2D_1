#pragma once
class IRenderer
{
public:
	IRenderer() = default;
	~IRenderer() = default ;

	virtual void Render() = 0;
};