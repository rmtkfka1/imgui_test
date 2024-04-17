#pragma once

class Texture;

class Mesh
{
public:

	Mesh();
	~Mesh();

	void Init(vector<Vertex>& vec, vector<uint32>& index);
	void Render();
	void SetTexture(shared_ptr<Texture> tex) { _tex = tex; }

private:
	void CreateVertxBuffer(vector<Vertex>& vec);
	void CreateIndexBuffer(vector<uint32>& index);


private:
	ComPtr<ID3D12Resource>		_vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW	_vertexBufferView = {};
	uint32 _vertexCount = 0;


	ComPtr<ID3D12Resource>		_indexBuffer;
	D3D12_INDEX_BUFFER_VIEW	_indexBufferView = {};
	uint32 _indexCount = 0;


	cb test;
	cb test2;

	shared_ptr<Texture> _tex = {};

};

