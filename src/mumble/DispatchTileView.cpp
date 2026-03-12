// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "DispatchTileView.h"

#include "DispatchTileDelegate.h"

DispatchTileView::DispatchTileView(QWidget *parent) : QListView(parent) {
	setObjectName(QLatin1String("qtvDispatch"));
	setViewMode(QListView::IconMode);
	setFlow(QListView::LeftToRight);
	setWrapping(true);
	setResizeMode(QListView::Adjust);
	setMovement(QListView::Static);
	setSpacing(8);
	setSelectionMode(QAbstractItemView::SingleSelection);
	setUniformItemSizes(true);
	setContextMenuPolicy(Qt::CustomContextMenu);
	setItemDelegate(new DispatchTileDelegate(this));
}

DispatchTileView::~DispatchTileView() = default;
