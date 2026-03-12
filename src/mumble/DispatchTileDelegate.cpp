// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "DispatchTileDelegate.h"

#include <QtGui/QPainter>

DispatchTileDelegate::DispatchTileDelegate(QObject *parent) : QStyledItemDelegate(parent) {}

DispatchTileDelegate::~DispatchTileDelegate() = default;

void DispatchTileDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
							 const QModelIndex &index) const {
	painter->save();
	painter->setRenderHint(QPainter::Antialiasing, true);

	QRect tileRect = option.rect.adjusted(4, 4, -4, -4);
	QColor backgroundColor = option.state.testFlag(QStyle::State_Selected)
							 ? option.palette.highlight().color()
							 : option.palette.base().color().darker(108);
	QColor borderColor = option.palette.mid().color();

	painter->setPen(borderColor);
	painter->setBrush(backgroundColor);
	painter->drawRoundedRect(tileRect, 6.0, 6.0);

	QRect textRect = tileRect.adjusted(10, 8, -10, -8);
	QString label = index.data(Qt::DisplayRole).toString();
	if (label.isEmpty()) {
		label = tr("Dispatch tile");
	}

	painter->setPen(option.palette.text().color());
	painter->drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap, label);

	painter->restore();
}

QSize DispatchTileDelegate::sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const {
	return QSize(180, 110);
}
