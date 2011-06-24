#include "Material.h"
#include "Textures/Texture.h"
#include "Math/Matrix.h"

void Material::applyNormalMap(ShadeRecord& sr) {
   Matrix tangentMatrix(sr.dpdu, sr.dpdv, sr.normal);
   tangentMatrix.invert();

   Color color = normalMap->getColor(sr);
   Vector3D mapNormal(2.0 * color.red - 1.0, 2.0 * color.green - 1.0, 2.0 * color.blue - 1.0);
   sr.normal = tangentMatrix * mapNormal;
}
