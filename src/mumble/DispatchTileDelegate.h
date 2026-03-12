// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_DISPATCHTILEDELEGATE_H_
#define MUMBLE_MUMBLE_DISPATCHTILEDELEGATE_H_

#include <QtWidgets/QStyledItemDelegate>

class DispatchTileDelegate : public QStyledItemDelegate {
	Q_OBJECT
	Q_DISABLE_COPY(DispatchTileDelegate)

public:
	explicit DispatchTileDelegate(QObject *parent = nullptr);
	~DispatchTileDelegate() Q_DECL_OVERRIDE;

	void paint(QPainter *painter, const QStyleOptionViewItem &option,
			   const QModelIndex &index) const Q_DECL_OVERRIDE;
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
};

#endif // MUMBLE_MUMBLE_DISPATCHTILEDELEGATE_H_
