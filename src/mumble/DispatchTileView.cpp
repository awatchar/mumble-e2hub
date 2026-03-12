// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "DispatchTileView.h"

#include "DispatchTileDelegate.h"

#include <QtCore/QSize>
#include <QtWidgets/QStyleOptionViewItem>

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
	setTileSizePreset(TileSizePreset::Normal);
}

DispatchTileView::~DispatchTileView() = default;

void DispatchTileView::setTileSizePreset(TileSizePreset preset) {
	m_tileSizePreset = preset;

	DispatchTileDelegate *dispatchDelegate = qobject_cast< DispatchTileDelegate * >(itemDelegate());
	if (dispatchDelegate) {
		dispatchDelegate->setTileSizePreset(preset == TileSizePreset::Large
							   ? DispatchTileDelegate::TileSizePreset::Large
							   : DispatchTileDelegate::TileSizePreset::Normal);

		const QSize tileSize = dispatchDelegate->sizeHint(QStyleOptionViewItem(), QModelIndex());
		setGridSize(tileSize + QSize(16, 16));
	} else {
		setGridSize(preset == TileSizePreset::Large ? QSize(276, 166) : QSize(226, 140));
	}
	viewport()->update();
}

DispatchTileView::TileSizePreset DispatchTileView::tileSizePreset() const {
	return m_tileSizePreset;
}
