#pragma once

#include "renderer/material.h"

class Texture;

class BasicMaterial : public Material
{
	BasicShader* m_BasicShader;

protected:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_SamplerState;

	Ref<ImageResourceFile> m_DiffuseImageFile;
	Ref<ImageResourceFile> m_NormalImageFile;
	Ref<ImageResourceFile> m_SpecularImageFile;
	Ref<ImageResourceFile> m_LightmapImageFile;

	bool m_IsLit;
	bool m_IsNormal;
	Color m_Color;
	float m_SpecularIntensity;
	float m_SpecularPower;
	float m_Reflectivity;
	float m_RefractionConstant;
	float m_Refractivity;
	bool m_IsAffectedBySky;

	void setPSConstantBuffer(const PSDiffuseConstantBufferMaterial& constantBuffer);
	void setVSConstantBuffer(const VSDiffuseConstantBuffer& constantBuffer);

public:
	const static inline String s_MaterialName = "BasicMaterial";
	enum class VertexConstantBufferType
	{
		Model,
		End
	};
	enum class PixelConstantBufferType
	{
		Material,
		End
	};

	BasicMaterial() = delete;
	BasicMaterial(bool isAlpha, const String& imagePath, const String& normalImagePath, const String& specularImagePath, const String& lightmapImagePath, bool isNormal, Color color, bool isLit, float specularIntensity, float specularPower, float reflectivity, float refractionConstant, float refractivity, bool affectedBySky);
	~BasicMaterial() = default;

	void setColor(const Color& color) { m_Color = color; };
	void setDiffuseTexture(Ref<ImageResourceFile> image);
	void setNormalTexture(Ref<ImageResourceFile> image);
	void setSpecularTexture(Ref<ImageResourceFile> image);
	void setLightmapTexture(Ref<ImageResourceFile> image);
	void removeNormal();
	void setSpecularIntensity(float specIntensity) { m_SpecularIntensity = specIntensity; }
	void setSpecularPower(float specPower) { m_SpecularPower = specPower; }

	static Material* CreateDefault();
	static Material* Create(const JSON::json& materialData);

	virtual ID3D11ShaderResourceView* getPreview() override;

	virtual void bind() override;
	JSON::json getJSON() const override;

	void draw() override;
};
