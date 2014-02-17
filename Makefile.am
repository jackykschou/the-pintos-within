ACLOCAL_AMFLAGS= -I m4
noinst_HEADERS= BaseApplication.h Vector3.h Singleton.h PhysicsManager.h transform.h Rigidbody.h common.h Vector3.h GameObject.h Collider.h SphereRigidbody.h

bin_PROGRAMS= OgreApp
OgreApp_CPPFLAGS= -I$(top_srcdir) -std=c++0x
OgreApp_SOURCES= BaseApplication.cpp Singleton.cpp PhysicsManager.cpp Rigidbody.cpp GameObject.cpp Collider.cpp SphereRigidbody.cpp
OgreApp_CXXFLAGS= $(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS)
OgreApp_LDADD= $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS)

EXTRA_DIST= buildit makeit
AUTOMAKE_OPTIONS= foreign
