// Filename: collisionSolid.h
// Created by:  drose (24Apr00)
//
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) 2001, Disney Enterprises, Inc.  All rights reserved
//
// All use of this software is subject to the terms of the Panda 3d
// Software license.  You should have received a copy of this license
// along with this source code; you will also find a current copy of
// the license at http://www.panda3d.org/license.txt .
//
// To contact the maintainers of this program write to
// panda3d@yahoogroups.com .
//
////////////////////////////////////////////////////////////////////

#ifndef COLLISIONSOLID_H
#define COLLISIONSOLID_H

#include "pandabase.h"

#include "typedWritableReferenceCount.h"
#include "boundedObject.h"
#include "luse.h"
#include "pointerTo.h"
#include "renderState.h"
#include "qpgeomNode.h"

class qpCollisionHandler;
class qpCollisionEntry;
class CollisionSphere;
class qpGeomNode;
class qpCollisionNode;

///////////////////////////////////////////////////////////////////
//       Class : CollisionSolid
// Description : The abstract base class for all things that can
//               collide with other things in the world, and all the
//               things they can collide with (except geometry).
//
//               This class and its derivatives really work very
//               similarly to the way BoundingVolume and all of its
//               derivatives work.  There's a different subclass for
//               each basic shape of solid, and double-dispatch
//               function calls handle the subset of the N*N
//               intersection tests that we care about.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDA CollisionSolid :
  public TypedWritableReferenceCount, public BoundedObject {
public:
  CollisionSolid();
  CollisionSolid(const CollisionSolid &copy);
  virtual ~CollisionSolid();

  virtual CollisionSolid *make_copy()=0;
  virtual LPoint3f get_collision_origin() const=0;

PUBLISHED:
  INLINE void set_tangible(bool tangible);
  INLINE bool is_tangible() const;

public:
  virtual int
  test_intersection(qpCollisionHandler *record,
                    const qpCollisionEntry &entry,
                    const CollisionSolid *into) const=0;

  virtual void xform(const LMatrix4f &mat)=0;

  qpGeomNode *get_viz();

PUBLISHED:
  virtual void output(ostream &out) const;
  virtual void write(ostream &out, int indent_level = 0) const;

protected:
  virtual int
  test_intersection_from_sphere(qpCollisionHandler *record,
                                const qpCollisionEntry &entry) const;
  virtual int
  test_intersection_from_ray(qpCollisionHandler *record,
                             const qpCollisionEntry &entry) const;
  virtual int
  test_intersection_from_segment(qpCollisionHandler *record,
                                 const qpCollisionEntry &entry) const;

  static void
  report_undefined_intersection_test(TypeHandle from_type,
                                     TypeHandle into_type);

  INLINE void mark_viz_stale();
  virtual void fill_viz_geom();

  CPT(RenderState) get_solid_viz_state();
  CPT(RenderState) get_wireframe_viz_state();
  CPT(RenderState) get_other_viz_state();

  PT(qpGeomNode) _viz_geom;
  bool _viz_geom_stale;
  bool _tangible;

public:
  virtual void write_datagram(BamWriter* manager, Datagram &me);

protected:
  void fillin(DatagramIterator& scan, BamReader* manager);

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    TypedWritableReferenceCount::init_type();
    BoundedObject::init_type();
    register_type(_type_handle, "CollisionSolid",
                  TypedWritableReferenceCount::get_class_type(),
                  BoundedObject::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;

  friend class CollisionSphere;
  friend class CollisionRay;
  friend class CollisionSegment;
};

INLINE ostream &operator << (ostream &out, const CollisionSolid &cs) {
  cs.output(out);
  return out;
}

#include "collisionSolid.I"

#endif

