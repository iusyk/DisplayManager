/*
 *  IlmObject
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 * Copyright (C) 2017 EPAM Systems Inc.
 */

#include "IlmObject.hpp"

#include <algorithm>
#include <vector>

#include "Exception.hpp"
#include "ObjectManager.hpp"

using std::find;
using std::find_if;
using std::list;
using std::string;
using std::to_string;
using std::vector;

/*******************************************************************************
 * IlmObject
 ******************************************************************************/

IlmObject::IlmObject(ObjectManager& manager, const string& type,
                     const string& name, t_ilm_uint id)
    : mManager(manager),
      mName(name),
      mID(id),
      mOrder(0),
      mLog(type + "[" + to_string(id) + "]")
{
    LOG(mLog, DEBUG) << "Create";
}

IlmObject::~IlmObject()
{
    LOG(mLog, DEBUG) << "Delete";

    if (mParent) {
        mParent->removeChild(mID);
        mManager.addToUpdateList(mParent);
    }

    mParent.reset();
}

/*******************************************************************************
 * Public
 ******************************************************************************/

void IlmObject::setOrder(int order)
{
    LOG(mLog, DEBUG) << "Set order " << order;

    if (mParent) {
        mParent->removeChildFromList(mID);
        mParent->addChildToList(mID, order);

        mManager.addToUpdateList(mParent);
    }

    mOrder = order;
}

void IlmObject::setParent(IlmObjectPtr parent)
{
    if (parent) {
        LOG(mLog, DEBUG) << "Set parent " << parent->getName();
    }
    else {
        LOG(mLog, DEBUG) << "Remove parent";
    }

    if (mParent) {
        mParent->removeChild(mID);

        mManager.addToUpdateList(mParent);
    }

    mParent = parent;

    if (mParent) {
        mParent->addChild(mID, mOrder);

        mManager.addToUpdateList(mParent);
    }
}

void IlmObject::update()
{
    LOG(mLog, DEBUG) << "Update";

    vector<t_ilm_uint> ids;

    for (auto child : mChildren) {
        ids.push_back(child.id);
    }

    onUpdate(ids);
}

/*******************************************************************************
 * Private
 ******************************************************************************/

void IlmObject::addChild(t_ilm_uint id, int order)
{
    onAddChild(id);

    addChildToList(id, order);
}

void IlmObject::addChildToList(t_ilm_uint id, int order)
{
    for (auto it = mChildren.begin(); it != mChildren.end(); it++) {
        if (order < it->order) {
            mChildren.insert(it, {id, order});

            return;
        }
    }

    mChildren.push_back({id, order});
}

void IlmObject::removeChild(t_ilm_uint id)
{
    onRemoveChild(id);

    removeChildFromList(id);
}

void IlmObject::removeChildFromList(t_ilm_uint id)
{
    auto it = find_if(mChildren.begin(), mChildren.end(),
                      [id](const Child& child) { return child.id == id; });

    if (it != mChildren.end()) {
        mChildren.erase(it);
    }
}
