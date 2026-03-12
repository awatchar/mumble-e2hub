// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "DispatchProxyModel.h"

#include "Channel.h"
#include "ClientUser.h"
#include "DispatchRoles.h"
#include "UserModel.h"

DispatchProxyModel::DispatchProxyModel(QObject *parent) : QAbstractProxyModel(parent) {}

DispatchProxyModel::~DispatchProxyModel() = default;

QModelIndex DispatchProxyModel::mapToSource(const QModelIndex &proxyIndex) const {
	if (!proxyIndex.isValid() || proxyIndex.column() != 0 || proxyIndex.row() < 0 || proxyIndex.row() >= m_userItems.size()) {
		return QModelIndex();
	}

	return m_userItems.at(proxyIndex.row());
}

QModelIndex DispatchProxyModel::mapFromSource(const QModelIndex &sourceIndex) const {
	if (!sourceIndex.isValid() || sourceIndex.column() != 0) {
		return QModelIndex();
	}

	for (int row = 0; row < m_userItems.size(); ++row) {
		if (m_userItems.at(row) == sourceIndex) {
			return createIndex(row, 0);
		}
	}

	return QModelIndex();
}

QModelIndex DispatchProxyModel::index(int row, int column, const QModelIndex &parent) const {
	if (parent.isValid() || column != 0 || row < 0 || row >= m_userItems.size()) {
		return QModelIndex();
	}

	return createIndex(row, column);
}

QModelIndex DispatchProxyModel::parent(const QModelIndex &) const {
	return QModelIndex();
}

int DispatchProxyModel::rowCount(const QModelIndex &parent) const {
	if (parent.isValid()) {
		return 0;
	}

	return m_userItems.size();
}

int DispatchProxyModel::columnCount(const QModelIndex &parent) const {
	Q_UNUSED(parent);
	return 1;
}

QVariant DispatchProxyModel::data(const QModelIndex &index, int role) const {
	if (!index.isValid() || !m_userModel) {
		return QVariant();
	}

	QModelIndex sourceIndex = mapToSource(index);
	if (!sourceIndex.isValid()) {
		return QVariant();
	}

	ClientUser *user = m_userModel->getUser(sourceIndex);
	if (!user) {
		return QVariant();
	}

	Channel *parentChannel = m_userModel->getChannel(sourceIndex.parent());

	switch (role) {
		case Qt::DisplayRole:
			return UserModel::createDisplayString(*user, false, parentChannel);
		case DispatchRoles::DisplayName:
			return user->qsName;
		case DispatchRoles::ChannelName:
			return parentChannel ? parentChannel->qsName : QString();
		case DispatchRoles::SessionId:
			return static_cast< uint >(user->uiSession);
		case DispatchRoles::TalkingState:
			return static_cast< int >(user->tsState);
		case DispatchRoles::Muted:
			return user->bMute || user->bSelfMute;
		case DispatchRoles::Deafened:
			return user->bDeaf || user->bSelfDeaf;
		case DispatchRoles::LocalMuted:
			return user->bLocalMute;
		default:
			return sourceModel() ? sourceModel()->data(sourceIndex, role) : QVariant();
	}
}

void DispatchProxyModel::setSourceModel(QAbstractItemModel *sourceModel) {
	if (this->sourceModel()) {
		disconnect(this->sourceModel(), nullptr, this, nullptr);
	}

	QAbstractProxyModel::setSourceModel(sourceModel);
	m_userModel = qobject_cast< UserModel * >(sourceModel);

	if (sourceModel) {
		auto rebuildFn = [this]() { rebuildIndex(); };
		connect(sourceModel, &QAbstractItemModel::modelReset, this, rebuildFn);
		connect(sourceModel, &QAbstractItemModel::layoutChanged, this, rebuildFn);
		connect(sourceModel, &QAbstractItemModel::rowsInserted, this, rebuildFn);
		connect(sourceModel, &QAbstractItemModel::rowsRemoved, this, rebuildFn);
		connect(sourceModel, &QAbstractItemModel::rowsMoved, this, rebuildFn);
		connect(sourceModel, &QAbstractItemModel::dataChanged, this, rebuildFn);
	}

	rebuildIndex();
}

void DispatchProxyModel::rebuildIndex() {
	beginResetModel();
	m_userItems.clear();

	if (sourceModel() && m_userModel) {
		collectUsers(QModelIndex());
	}

	endResetModel();
}

void DispatchProxyModel::collectUsers(const QModelIndex &parent) {
	QAbstractItemModel *source = sourceModel();
	if (!source || !m_userModel) {
		return;
	}

	const int rows = source->rowCount(parent);
	for (int row = 0; row < rows; ++row) {
		QModelIndex idx = source->index(row, 0, parent);
		if (!idx.isValid()) {
			continue;
		}

		ClientUser *user = m_userModel->getUser(idx);
		if (user && !m_userModel->isChannelListener(idx)) {
			m_userItems.push_back(QPersistentModelIndex(idx));
		}

		collectUsers(idx);
	}
}
