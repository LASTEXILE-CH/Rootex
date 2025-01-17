#include "material_library.h"

#include "core/resource_loader.h"
#include "core/resource_files/text_resource_file.h"

MaterialLibrary::MaterialMap MaterialLibrary::s_Materials;
const String MaterialLibrary::s_DefaultMaterialPath = "rootex/assets/materials/default.rmat";
const String MaterialLibrary::s_DefaultParticlesMaterialPath = "rootex/assets/materials/default_particles.rmat";
const String MaterialLibrary::s_AnimatedDefaultMaterialPath = "rootex/assets/materials/animated_default.rmat";

MaterialLibrary::MaterialDatabase MaterialLibrary::s_MaterialDatabase = {
	{ BasicMaterial::s_MaterialName, { BasicMaterial::CreateDefault, BasicMaterial::Create } },
	{ ParticlesMaterial::s_MaterialName, { ParticlesMaterial::CreateDefault, ParticlesMaterial::Create } },
	{ SkyMaterial::s_MaterialName, { SkyMaterial::CreateDefault, SkyMaterial::Create } },
	{ AnimatedMaterial::s_MaterialName, { AnimatedMaterial::CreateDefault, AnimatedMaterial::Create } }
};

bool MaterialLibrary::IsDefault(const String& materialPath)
{
	static String rootex = "rootex";
	return materialPath.substr(0, rootex.size()) == rootex;
}

Ref<Material> MaterialLibrary::GetMaterial(const String& materialPath)
{
	if (s_Materials.find(materialPath) == s_Materials.end())
	{
		Ref<TextResourceFile> materialResourceFile = ResourceLoader::CreateTextResourceFile(materialPath);
		if (materialResourceFile)
		{
			const JSON::json& materialJSON = JSON::json::parse(materialResourceFile->getString());
			Ref<Material> material(s_MaterialDatabase[materialJSON["type"]].second(materialJSON));
			material->setFileName(materialPath);
			s_Materials[materialPath] = { (String)materialJSON["type"], material };
			return material;
		}

		WARN("Material file not found, returning default material instead of: " + materialPath);
		return GetDefaultMaterial();
	}
	else
	{
		if (Ref<Material> lockedMaterial = s_Materials[materialPath].second.lock())
		{
			return lockedMaterial;
		}
		else
		{
			Ref<TextResourceFile> materialFile = ResourceLoader::CreateTextResourceFile(materialPath);
			const JSON::json materialJSON = JSON::json::parse(materialFile->getString());
			Ref<Material> material(s_MaterialDatabase[materialJSON["type"]].second(materialJSON));
			material->setFileName(materialPath);
			s_Materials[materialPath].second = material;
			return material;
		}
	}
}

Ref<Material> MaterialLibrary::GetDefaultMaterial()
{
	if (Ref<Material> lockedMaterial = s_Materials[s_DefaultMaterialPath].second.lock())
	{
		return lockedMaterial;
	}
	else
	{
		Ref<Material> material(BasicMaterial::CreateDefault());
		material->setFileName(s_DefaultMaterialPath);
		s_Materials[s_DefaultMaterialPath].second = material;
		return material;
	}
}

Ref<Material> MaterialLibrary::GetDefaultParticlesMaterial()
{
	if (Ref<Material> lockedMaterial = s_Materials[s_DefaultParticlesMaterialPath].second.lock())
	{
		return lockedMaterial;
	}
	else
	{
		Ref<Material> material(ParticlesMaterial::CreateDefault());
		material->setFileName(s_DefaultParticlesMaterialPath);
		s_Materials[s_DefaultParticlesMaterialPath].second = material;
		return material;
	}
}

Ref<Material> MaterialLibrary::GetDefaultAnimatedMaterial()
{
	if (Ref<Material> lockedMaterial = s_Materials[s_AnimatedDefaultMaterialPath].second.lock())
	{
		return lockedMaterial;
	}
	else
	{
		Ref<Material> material(AnimatedMaterial::CreateDefault());
		material->setFileName(s_AnimatedDefaultMaterialPath);
		s_Materials[s_AnimatedDefaultMaterialPath].second = material;
		return material;
	}
}

void MaterialLibrary::SaveAll()
{
	for (auto& [materialPath, materialInfo] : s_Materials)
	{
		if (IsDefault(materialPath))
		{
			continue;
		}
		if (Ref<Material> lockedMaterial = materialInfo.second.lock())
		{
			Ref<TextResourceFile> materialFile = ResourceLoader::CreateNewTextResourceFile(materialPath);
			materialFile->putString(lockedMaterial->getJSON().dump(4));
			materialFile->save();
		}
	}
}

void MaterialLibrary::CreateNewMaterialFile(const String& materialPath, const String& materialType)
{
	if (materialPath == s_DefaultMaterialPath)
	{
		return;
	}

	if (s_Materials.find(materialPath) == s_Materials.end())
	{
		if (!OS::IsExists(materialPath))
		{
			Ref<TextResourceFile> materialFile = ResourceLoader::CreateNewTextResourceFile(materialPath);
			Ref<Material> material(s_MaterialDatabase[materialType].first());
			materialFile->putString(material->getJSON().dump(4));
			materialFile->save();
			s_Materials[materialPath] = { materialType, {} };
			PRINT("Created material: " + materialPath + " of type " + materialType);

			return;
		}
	}
}
