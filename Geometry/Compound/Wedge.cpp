#include "Wedge.h"
#include "Parser/Hash.h"
#include "Geometry/Annulus.h"
#include "Geometry/Sphere.h"
#include "Geometry/Cylinder.h"
#include "Geometry/Torus.h"
#include "Geometry/Rectangle.h"
#include "Geometry/Instance.h"
#include "Math/Maths.h"

Wedge::Wedge() {
}

Wedge::Wedge(float ir, float outr, float br, float a1, float a2, float y1, float y2) :
   innerR(ir),
   outerR(outr),
   bevelR(br),
   angle1(a1),
   angle2(a2),
   minY(y1),
   maxY(y2),
   cover(true)
{
   build();
}

Wedge::~Wedge() {
}

void Wedge::setHash(Hash* hash) {
   innerR = hash->getDouble("innerRadius");
   outerR = hash->getDouble("outerRadius");
   bevelR = hash->getDouble("bevelRadius");
   angle1 = hash->getDouble("angle1");
   angle2 = hash->getDouble("angle2");
   minY = hash->getDouble("minY");
   maxY = hash->getDouble("maxY");
   
   cover = true;
   if(hash->contains("cover")) {
      if(hash->getString("cover") == "false") {
         cover = false;
      }
   }
   
   build();
   
   setupMaterial(hash->getValue("material")->getHash());
}
 
void Wedge::build() {
   double sinAngle1 = sin(angle1 * DEG_TO_RAD);
   double cosAngle1 = cos(angle1 * DEG_TO_RAD);
   double sinAngle2 = sin(angle2 * DEG_TO_RAD);
   double cosAngle2 = cos(angle2 * DEG_TO_RAD);

   // The following values are used to offset objects from the edge of the wedge
	double sin_alpha = bevelR / (innerR + bevelR);
	double cos_alpha = sqrt(innerR * innerR + 2.0 * innerR * bevelR) / (innerR + bevelR);
	double sin_beta = bevelR / (outerR - bevelR);
	double cos_beta = sqrt(outerR * outerR - 2.0 * outerR * bevelR) / (outerR - bevelR);

	double alpha = acos(cos_alpha) * RAD_TO_DEG;
	double beta = acos(cos_beta) * RAD_TO_DEG;
	
   double s1 = sqrt(innerR * innerR + 2.0 * innerR * bevelR);
   double s2 = sqrt(outerR * outerR - 2.0 * outerR * bevelR);

   double xc1 = (innerR + bevelR) * (sinAngle1 * cos_alpha + cosAngle1 * sin_alpha);
   double zc1 = (innerR + bevelR) * (cosAngle1 * cos_alpha - sinAngle1 * sin_alpha);
   double xc2 = (outerR - bevelR) * (sinAngle1 * cos_beta + cosAngle1 * sin_beta);
   double zc2 = (outerR - bevelR) * (cosAngle1 * cos_beta - sinAngle1 * sin_beta);
	double xc3 = (innerR + bevelR) * (sinAngle2 * cos_alpha - cosAngle2 * sin_alpha);
	double zc3 = (innerR + bevelR) * (cosAngle2 * cos_alpha + sinAngle2 * sin_alpha);
	double xc4 = (outerR - bevelR) * (sinAngle2 * cos_beta - cosAngle2 * sin_beta);
	double zc4 = (outerR - bevelR) * (cosAngle2 * cos_beta + sinAngle2 * sin_beta);


   // BASE
   
   // Use an annulus for the base plate
   if(cover) {
      Annulus* base = new Annulus(Point3D(0, minY, 0), Vector3D(0, -1, 0), innerR + bevelR, outerR - bevelR);
      base->setAngleRange(angle1 + alpha, angle2 - alpha);
      objects.push_back(base);

      // Side patches
      Annulus* side1 = new Annulus(Point3D(0, minY, 0), Vector3D(0, -1, 0), 0.0, s2 - s1);
      side1->setAngleRange(0.0, alpha);
      Instance* patch1 = new Instance(side1);
      patch1->rotateY(angle1);
      patch1->translate(xc1, 0.0, zc1);
      objects.push_back(patch1);

      Annulus* side2 = new Annulus(Point3D(0, minY, 0), Vector3D(0, -1, 0), 0.0, s2 - s1);
      side2->setAngleRange(360.0 - alpha, 360.0);
      Instance* patch2 = new Instance(side2);
      patch2->rotateY(angle2);
      patch2->translate(xc3, 0.0, zc3);
      objects.push_back(patch2);
   }

   // Base spheres
   objects.push_back(new Sphere(Point3D(xc1, minY + bevelR, zc1), bevelR));
   objects.push_back(new Sphere(Point3D(xc2, minY + bevelR, zc2), bevelR));
   objects.push_back(new Sphere(Point3D(xc3, minY + bevelR, zc3), bevelR));
   objects.push_back(new Sphere(Point3D(xc4, minY + bevelR, zc4), bevelR));
   
   // Base cylinders
   Instance* baseCylinder1 = new Instance(new Cylinder(bevelR, 0.0, s2 - s1));
   baseCylinder1->rotateX(90.0);
   baseCylinder1->rotateY(angle1);
   baseCylinder1->translate(xc1, minY + bevelR, zc1);
   objects.push_back(baseCylinder1);

   Instance* baseCylinder2 = new Instance(new Cylinder(bevelR, 0.0, s2 - s1));
   baseCylinder2->rotateX(90.0);
   baseCylinder2->rotateY(angle2);
   baseCylinder2->translate(xc3, minY + bevelR, zc3);
   objects.push_back(baseCylinder2);
   
   // Base tori
   Torus* baseTorus1 = new Torus(innerR + bevelR, bevelR);
   baseTorus1->setPhiRange(angle1 + alpha, angle2 - alpha);
   Instance* bti1 = new Instance(baseTorus1);
   bti1->translate(0.0, minY + bevelR, 0.0);
   objects.push_back(bti1);
   
   Torus* baseTorus2 = new Torus(outerR - bevelR, bevelR);
   baseTorus2->setPhiRange(angle1 + beta, angle2 - beta);
   Instance* bti2 = new Instance(baseTorus2);
   bti2->translate(0.0, minY + bevelR, 0.0);
   objects.push_back(bti2);
   
   // SIDES

   // Side Cylinders
   Instance* sideCyl1 = new Instance(new Cylinder(bevelR, minY + bevelR, maxY - bevelR));
   sideCyl1->translate(xc1, 0.0, zc1);
   objects.push_back(sideCyl1);

   Instance* sideCyl2 = new Instance(new Cylinder(bevelR, minY + bevelR, maxY - bevelR));
   sideCyl2->translate(xc2, 0.0, zc2);
   objects.push_back(sideCyl2);

   Instance* sideCyl3 = new Instance(new Cylinder(bevelR, minY + bevelR, maxY - bevelR));
   sideCyl3->translate(xc3, 0.0, zc3);
   objects.push_back(sideCyl3);

   Instance* sideCyl4 = new Instance(new Cylinder(bevelR, minY + bevelR, maxY - bevelR));
   sideCyl4->translate(xc4, 0.0, zc4);
   objects.push_back(sideCyl4);
   
   if(cover) {
      // Side Rectanlges
      Point3D p1(s1 * sinAngle1, minY + bevelR, s1 * cosAngle1);
      Point3D p2(s2 * sinAngle1, minY + bevelR, s2 * cosAngle1);
      Vector3D a = p2 - p1;
      Vector3D b(0.0, maxY - minY - 2.0 * bevelR, 0.0);
      objects.push_back(new Rectangle(p1, a, b));
   
      p1.set(s1 * sinAngle2, minY + bevelR, s1 * cosAngle2);
      p2.set(s2 * sinAngle2, minY + bevelR, s2 * cosAngle2);
      a = p1 - p2;
      objects.push_back(new Rectangle(p2, a, b));
   
      // Cover Cylinders
      Cylinder* sideC1 = new Cylinder(innerR, minY + bevelR, maxY - bevelR);
      sideC1->setAngleRange(angle1 + alpha, angle2 - alpha);
      objects.push_back(sideC1);

      Cylinder* sideC2 = new Cylinder(outerR, minY + bevelR, maxY - bevelR);
      sideC2->setAngleRange(angle1 + beta, angle2 - beta);
      objects.push_back(sideC2);
   }

   // TOP
   
   // Top spheres
   objects.push_back(new Sphere(Point3D(xc1, maxY - bevelR, zc1), bevelR));
   objects.push_back(new Sphere(Point3D(xc2, maxY - bevelR, zc2), bevelR));
   objects.push_back(new Sphere(Point3D(xc3, maxY - bevelR, zc3), bevelR));
   objects.push_back(new Sphere(Point3D(xc4, maxY - bevelR, zc4), bevelR));

   // Top cylinders
   Instance* topCylinder1 = new Instance(new Cylinder(bevelR, 0.0, s2 - s1));
   topCylinder1->rotateX(90.0);
   topCylinder1->rotateY(angle1);
   topCylinder1->translate(xc1, maxY - bevelR, zc1);
   objects.push_back(topCylinder1);

   Instance* topCylinder2 = new Instance(new Cylinder(bevelR, 0.0, s2 - s1));
   topCylinder2->rotateX(90.0);
   topCylinder2->rotateY(angle2);
   topCylinder2->translate(xc3, maxY - bevelR, zc3);
   objects.push_back(topCylinder2);

   // Top tori
   Torus* topTorus1 = new Torus(innerR + bevelR, bevelR);
   topTorus1->setPhiRange(angle1 + alpha, angle2 - alpha);
   Instance* tti1 = new Instance(topTorus1);
   tti1->translate(0.0, maxY - bevelR, 0.0);
   objects.push_back(tti1);

   Torus* topTorus2 = new Torus(outerR - bevelR, bevelR);
   topTorus2->setPhiRange(angle1 + beta, angle2 - beta);
   Instance* tti2 = new Instance(topTorus2);
   tti2->translate(0.0, maxY - bevelR, 0.0);
   objects.push_back(tti2);

   if(cover) {
      // Top surface
      Annulus* top = new Annulus(Point3D(0, maxY, 0), Vector3D(0, 1, 0), innerR + bevelR, outerR - bevelR);
      top->setAngleRange(angle1 + alpha, angle2 - alpha);
      objects.push_back(top);
   
      // Top patches
      Annulus* tap1 = new Annulus(Point3D(0, maxY, 0), Vector3D(0, 1, 0), 0.0, s2-s1);
      tap1->setAngleRange(0.0, alpha);
      Instance* patch3 = new Instance(tap1);
      patch3->rotateY(angle1);
      patch3->translate(xc1, 0.0, zc1);
      objects.push_back(patch3);
   
      Annulus* tap2 = new Annulus(Point3D(0, maxY, 0), Vector3D(0, 1, 0), 0.0, s2-s1);
      tap2->setAngleRange(360.0 - alpha, 360.0);
      Instance* patch4 = new Instance(tap2);
      patch4->rotateY(angle2);
      patch4->translate(xc3, 0.0, zc3);
      objects.push_back(patch4);
   }

   // Define the bounding box
   double x0 = min(xc1, min(xc2, min(xc3, xc4)));
   double z0 = min(zc1, min(zc2, min(zc3, zc4)));
   double x1 = max(xc1, max(xc2, max(xc3, xc4)));
   double z1 = max(zc1, max(zc2, max(zc3, zc4)));

   bbox.x0 = x0 - bevelR;
   bbox.y0 = minY;
   bbox.z0 = z0 - bevelR;
   bbox.x1 = x1 + bevelR;
   bbox.y1 = maxY;
   bbox.z1 = z1 + bevelR;
   
   bool spans90 = angle1 < 90.0 && angle2 > 90.0;
   bool spans180 = angle1 < 180.0 && angle2 > 180.0;
   bool spans270 = angle1 < 270.0 && angle2 > 270.0;
   
   if(spans90 && spans180 && spans270) {
      bbox.x0 = bbox.z0 = -outerR;
      bbox.x1 = outerR;
      bbox.z1 = max(zc2, zc4);
   }
   else if(spans90 && spans180) {
      bbox.x0 = xc4 - bevelR;
      bbox.z0 = -outerR;
      bbox.x1 = outerR;
      bbox.z1 = zc2 + bevelR;
   }
   else if(spans180 && spans270) {
      bbox.x0 = bbox.z0 = -outerR;
      bbox.x1 = xc2 + bevelR;
      bbox.z1 = zc4 + bevelR;
   }
   else if(spans90) {
      bbox.x0 = min(xc1, xc3);
      bbox.z0 = zc4 - bevelR;
      bbox.x1 = outerR;
      bbox.z1 = zc2 + bevelR;
   }
   else if(spans180) {
      bbox.x0 = xc4 - bevelR;
      bbox.z0 = -outerR;
      bbox.x1 = xc2 + bevelR;
      bbox.z1 = max(zc1, zc3);
   }
   else if(spans270) {
      bbox.x0 = -outerR;
      bbox.z0 = zc2 - bevelR;
      bbox.x1 = max(xc1, xc3);
      bbox.z1 = zc4 + bevelR;
   }
}
