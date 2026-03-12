// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_DISPATCHPROXYMODEL_H_
#define MUMBLE_MUMBLE_DISPATCHPROXYMODEL_H_

#include <QtCore/QAbstractProxyModel>
#include <QtCore/QPersistentModelIndex>
#include <QtCore/QVector>

class UserModel;

class DispatchProxyModel : public QAbstractProxyModel {
	Q_OBJECT
	Q_DISABLE_COPY(DispatchProxyModel)

public:
	explicit DispatchProxyModel(QObject *parent = nullptr);
	~DispatchProxyModel() Q_DECL_OVERRIDE;

	QModelIndex mapToSource(const QModelIndex &proxyIndex) const Q_DECL_OVERRIDE;
	QModelIndex mapFromSource(const QModelIndex &sourceIndex) const Q_DECL_OVERRIDE;

	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
	QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
	int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
	int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

	void setSourceModel(QAbstractItemModel *sourceModel) Q_DECL_OVERRIDE;

private:
	QVector< QPersistentModelIndex > m_userItems;
	UserModel *m_userModel = nullptr;

	void rebuildIndex();
	void collectUsers(const QModelIndex &parent);
};

#endif // MUMBLE_MUMBLE_DISPATCHPROXYMODEL_H_
