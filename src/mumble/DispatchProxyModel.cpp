// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "DispatchProxyModel.h"

#include "Channel.h"
#include "ClientUser.h"
#include "DispatchRoles.h"
#include "UserModel.h"

#include <algorithm>

DispatchProxyModel::DispatchProxyModel(QObject *parent) : QAbstractProxyModel(parent) {}

DispatchProxyModel::~DispatchProxyModel() = default;

QModelIndex DispatchProxyModel::mapToSource(const QModelIndex &proxyIndex) const {
	if (!proxyIndex.isValid() || proxyIndex.column() != 0 || proxyIndex.row() < 0 || proxyIndex.row() >= m_userItems.size()) {
		return QModelIndex();
	}

	return m_userItems.at(proxyIndex.row());
}

QModelIndex DispatchProxyModel::mapFromSource(const QModelIndex &sourceIndex) const {
	if (!sourceIndex.isValid()) {
		return QModelIndex();
	}

	QModelIndex sourceColumnZero = sourceIndex.sibling(sourceIndex.row(), 0);
	if (!sourceColumnZero.isValid()) {
		return QModelIndex();
	}

	for (int row = 0; row < m_userItems.size(); ++row) {
		if (m_userItems.at(row) == sourceColumnZero) {
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
	// Intentionally conservative: rebuild the flattened cache on source-model changes.
	// This keeps mapping correctness simple while the Dispatch view is still evolving.
	beginResetModel();
	m_userItems.clear();

	if (sourceModel() && m_userModel) {
		collectUsers(QModelIndex());

		std::sort(m_userItems.begin(), m_userItems.end(), [this](const QPersistentModelIndex &lhs,
											 const QPersistentModelIndex &rhs) {
			ClientUser *leftUser  = m_userModel->getUser(lhs);
			ClientUser *rightUser = m_userModel->getUser(rhs);

			Channel *leftChannel  = m_userModel->getChannel(lhs.parent());
			Channel *rightChannel = m_userModel->getChannel(rhs.parent());

			const QString leftChannelName  = leftChannel ? leftChannel->qsName : QString();
			const QString rightChannelName = rightChannel ? rightChannel->qsName : QString();
			const int channelCmp           = QString::localeAwareCompare(leftChannelName, rightChannelName);
			if (channelCmp != 0) {
				return channelCmp < 0;
			}

			const QString leftDisplay  = leftUser ? leftUser->qsName : QString();
			const QString rightDisplay = rightUser ? rightUser->qsName : QString();
			const int userCmp          = QString::localeAwareCompare(leftDisplay, rightDisplay);
			if (userCmp != 0) {
				return userCmp < 0;
			}

			const uint leftSession  = leftUser ? leftUser->uiSession : 0;
			const uint rightSession = rightUser ? rightUser->uiSession : 0;
			return leftSession < rightSession;
		});
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
