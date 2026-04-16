#pragma once
#include"Device.h"
#include<vector>

class Resource final
{

	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> ResourceVec_{};
public:
};