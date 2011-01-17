#include "Reflective.h"
#include "BRDFs/PerfectSpecular.h"
#include "Tracer/Tracer.h"
#include "Parser/Hash.h"

Reflective::Reflective() : psBRDF(new PerfectSpecular) {
}

Reflective::~Reflective() {
   delete psBRDF;
}

void Reflective::setHash(Hash* hash) {
   Phong::setHash(hash);
   
   psBRDF->setKr(hash->getDouble("kr"));
   psBRDF->setColor(hash->getValue("reflectColor")->getArray());
}

Color Reflective::shade(ShadeRecord& sr, const Ray& ray) {
   Color L(Phong::shade(sr, ray));
   
   Vector3D wo = -ray.direction;
   Vector3D wi;
   Color fr = psBRDF->sample_f(sr, wo, wi);
   Ray reflectedRay(sr.hitPoint, wi);
   
   L += fr * sr.tracer->traceRay(reflectedRay, sr.depth+1) * sr.normal.dot(wi);
   return L;
}

Color Reflective::areaLightShade(ShadeRecord& sr, const Ray& ray) {
   return shade(sr, ray);
}
