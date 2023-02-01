#pragma once
class IndexBuffer
{
private:
	unsigned int m_id;
	unsigned int m_count;
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	IndexBuffer() = default;
	~IndexBuffer();

	void UpdateBuffer(const unsigned int* data, unsigned int count);
	void Bind()const;
	void UnBind()const;

	unsigned int GetCount()const;
};