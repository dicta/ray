#include "Material.h"
#include "Textures/ImageTexture.h"
#include "Math/Matrix.h"

Material::Material() : normalMap(NULL) {
}

Material::~Material() {
   if(normalMap != NULL) {
      delete normalMap;
   }
}

void Material::setNormalMap(string texName) {
   ImageTexture* tex = new ImageTexture();
   tex->setTextureFile(texName);
   normalMap = tex;
}

void Material::applyNormalMap(ShadeRecord& sr) {
   if(normalMap != NULL) {
      Matrix tangentMatrix(sr.dpdu, sr.dpdv, sr.normal);
      tangentMatrix.invert();

      Color color = normalMap->getColor(sr);
      Vector3D mapNormal(2.0 * color.red - 1.0, 2.0 * color.green - 1.0, 2.0 * color.blue - 1.0);
      mapNormal.normalize();
      sr.normal = tangentMatrix * mapNormal;
   }
}
