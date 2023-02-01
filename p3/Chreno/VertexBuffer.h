#pragma once
class VertexBuffer
{
private:
	unsigned int m_id;
public:
	VertexBuffer(const void* data, unsigned int size);
	VertexBuffer() = default;
	~VertexBuffer();

	void Bind()const;
	void UnBind()const;
};