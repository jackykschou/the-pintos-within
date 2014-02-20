ACLOCAL_AMFLAGS= -I m4
noinst_HEADERS= Vector3.h Singleton.h PhysicsManager.h InputManager.h 

bin_PROGRAMS= OgreApp
OgreApp_CPPFLAGS= -I$(top_srcdir) -std=c++0x
OgreApp_SOURCES= Singleton.cpp PhysicsManager.cpp
OgreApp_CXXFLAGS= $(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS) $(SDL_CFLAGS)
OgreApp_LDADD= $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS) $(SDL_LIBS)

EXTRA_DIST= buildit makeit
AUTOMAKE_OPTIONS= foreign

