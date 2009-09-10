/* The Butterfly Effect 
 * This file copyright (C) 2009  Klaas van Gend
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation
 * applicable version is GPL version 2 only.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "BaseObject.h"
#include "DrawObject.h"
#include "World.h"
#include "Box2D.h"
#include "BaseObjectSerializer.h"

// Static variables
static b2World* theStaticB2WorldPtr = NULL;

//   ***********************************************
//   *                                             *
//   * NOTE: the ObjectFactory class definition is *
//   *       below the BaseObject definition       *
//   *                                             *
//   ***********************************************


// Constructors/Destructors
//  

BaseObject::BaseObject ( ) 
{
	initAttributes();
}

BaseObject::~BaseObject ( ) 
{
	DEBUG5("~BaseObject() for %p\n", this);

	// destroy the Body
	//
	deletePhysicsObject();
	
	// destroy the ShapeDefs
	//
	clearShapeList();
	
	// destroy the BodyDef
	//
	if (theB2BodyDefPtr)
	{
		delete theB2BodyDefPtr;
		theB2BodyDefPtr=NULL;
	}

	// delete the corresponding DrawObject
	//
	if (theDrawObjectPtr)
	{
		delete theDrawObjectPtr;
		theDrawObjectPtr = NULL;
	}
}

//  
// Methods
//  


// Accessor methods
//  

b2World* BaseObject::getB2WorldPtr(void) const
{
	assert (theStaticB2WorldPtr);
	return theStaticB2WorldPtr;
}

Position BaseObject::getTempCenter (void) const
{
	// no physics object, no temp center
	if (isPhysicsObjectCreated()==false)
		return getOrigCenter();
	
	return Position(theB2BodyPtr->GetPosition().x,
					theB2BodyPtr->GetPosition().y,
					theB2BodyPtr->GetAngle());
}

void BaseObject::setTempCenter ( Position new_var )
{
	assert(isPhysicsObjectCreated());
	theB2BodyPtr->SetXForm(b2Vec2(new_var.x, new_var.y), new_var.angle);
}


void BaseObject::ForWorldOnly::setTheB2WorldPtr(b2World* aPtr)
{
	theStaticB2WorldPtr = aPtr;
}


// Other methods
//  


void BaseObject::clearShapeList()
{
	while(theShapeList.isEmpty()==false)
	{
		b2ShapeDef* mySDPtr = theShapeList.first();
		delete mySDPtr;
		theShapeList.pop_front();
	}
}

DrawObject*  BaseObject::createDrawObject(void)
{
	assert(theDrawObjectPtr==NULL);
	theDrawObjectPtr = new DrawObject(this);
	return theDrawObjectPtr;
}

void BaseObject::createPhysicsObject()
{
	DEBUG5("BaseObject::createPhysicsObject()\n");
	// first fixup the bodydef with the current position
	assert(theB2BodyDefPtr!=NULL);
	theB2BodyDefPtr->position.Set(theCenter.x, theCenter.y);
	theB2BodyDefPtr->angle = theCenter.angle;
	// do not set mass properties here - that will be done in MovableObject
	// (and as such is done already when we get here)
	
	// then create the body
	if (theB2BodyPtr!=NULL)
		deletePhysicsObject();
	theB2BodyPtr = getB2WorldPtr()->CreateBody(theB2BodyDefPtr);
	assert(theB2BodyPtr != NULL);
	
	// then create the shapes from the shapedefs
	ShapeList::const_iterator myI = theShapeList.begin();
	for (;myI != theShapeList.end(); ++myI)
	{
		(*myI)->restitution = theBounciness;
		theB2BodyPtr->CreateShape(*myI);
	}
	theB2BodyPtr->SetMassFromShapes();
	DEBUG5("Object %s has mass %f kg\n", getName().toAscii().constData(),
		theB2BodyPtr->GetMass());
}

void BaseObject::deletePhysicsObject()
{
	DEBUG5("BaseObject::deletePhysicsObject()\n");
	// have B2World destroy the body - that will automatically destroy
	// the shapes
	if (theB2BodyPtr!=NULL)
		getB2WorldPtr()->DestroyBody(theB2BodyPtr);
	theB2BodyPtr = NULL;
}


bool BaseObject::deregister(void)
{
	deletePhysicsObject();
	theWorldPtr->removeObject(this);
	if (theDrawObjectPtr)
		theDrawObjectPtr->deregister();
	// note that we keep the pointer - and will re-use it in the future if needed
	return true;
}

const BaseObjectSerializer* BaseObject::getSerializer(void) const
{
	return new BaseObjectSerializer(this);
}

void BaseObject::initAttributes ( ) 
{
	DEBUG5("BaseObject::initAttributes\n");
	theB2BodyDefPtr= new b2BodyDef();
	theB2BodyPtr=NULL;
	
	theWidth = 1.0;
	theHeight = 1.0;
	theBounciness = 1.0;
	// don't need to initialise theCenter - it has a default constructor
	
	theDrawObjectPtr = NULL;
	theWorldPtr = NULL;

	// TODO - we need to move this into Level, once Toolbox is implemented
	theIsMovable = true;
}

bool BaseObject::isSleeping() const
{
	if (isPhysicsObjectCreated()) 
		return theB2BodyPtr->IsSleeping(); 
	else 
		return false;
}

bool BaseObject::reregister(void)
{
	reset();
	theWorldPtr->addObject(this);
	if (theDrawObjectPtr == NULL)
		createDrawObject();
	theDrawObjectPtr->reregister();
	createPhysicsObject();
	return true;
}

void BaseObject::reset ( ) 
{
	DEBUG5("BaseObject::reset() body pos for '%s' to (%f,%f)@%f\n", 
			getName().toAscii().constData(), theCenter.x, theCenter.y, theCenter.angle);

	if(isPhysicsObjectCreated()==false)
		return;
	
	// reset the position
	setTempCenter(getOrigCenter());

	// reset the velocities and such
	theB2BodyPtr->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
	theB2BodyPtr->SetAngularVelocity(0.0f);
}


//////////////////////////////////////////////////////////////////////////////

/** small, only locally used class
  */
class FactoryList
{
public:
	void insert(const QString& aName, ObjectFactory* theFactoryPtr)
		{ theMap[aName]=theFactoryPtr; }
	const ObjectFactory* getFactoryPtr(const QString& aName) const
		{ return theMap.value(aName); }

	ObjectFactory::ObjectFactoryList* getAllFactories(void)
	{
		ObjectFactory::ObjectFactoryList* myList = new ObjectFactory::ObjectFactoryList();
		TheMap::iterator myI = theMap.begin();
		while (myI != theMap.end())
		{
			myList->push_back(*myI);
			++myI;
		}
		return myList;
	}

private:
	typedef QMap<QString, ObjectFactory*> TheMap;
	TheMap theMap;
};
static FactoryList* theFactoryListPtr=NULL;


void
ObjectFactory::announceObjectType(const QString& anObjectTypeName, ObjectFactory* aThisPtr)
{
	DEBUG4("ObjectFactory::announceObjectType(\"%s\", %p)\n", 
			anObjectTypeName.toAscii().constData(), aThisPtr);
	if (theFactoryListPtr==NULL)
		theFactoryListPtr = new FactoryList();
	theFactoryListPtr->insert(anObjectTypeName,aThisPtr);
}

BaseObject* 
ObjectFactory::createObject(
		const QString& aName, 
		const Position aPosition,
		const qreal aWidth,
		const qreal anHeight) const
{
	const ObjectFactory* myFactoryPtr = theFactoryListPtr->getFactoryPtr(aName);
	DEBUG5("ObjectFactory::createObject(\"%s\") is %p\n", aName.toAscii().constData(), myFactoryPtr);
	if (myFactoryPtr == NULL)
		return NULL;
	BaseObject* myObjectPtr = myFactoryPtr->createObject();
	myObjectPtr->setOrigCenter(aPosition);
	if (myObjectPtr->isResizable() & BaseObject::HORIZONTALRESIZE)
		myObjectPtr->setTheWidth(aWidth);
	if (myObjectPtr->isResizable() & BaseObject::VERTICALRESIZE)
		myObjectPtr->setTheHeight(anHeight);
	myObjectPtr->createPhysicsObject();
	return myObjectPtr;
}

ObjectFactory::ObjectFactoryList* ObjectFactory::getAllFactories(void)
{
	if (theFactoryListPtr==NULL)
		return NULL;
	return theFactoryListPtr->getAllFactories();
}
