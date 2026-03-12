// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_DISPATCHPROXYMODEL_H_
#define MUMBLE_MUMBLE_DISPATCHPROXYMODEL_H_

#include <QtCore/QIdentityProxyModel>

class DispatchProxyModel : public QIdentityProxyModel {
	Q_OBJECT
	Q_DISABLE_COPY(DispatchProxyModel)

public:
	explicit DispatchProxyModel(QObject *parent = nullptr);
	~DispatchProxyModel() Q_DECL_OVERRIDE;
};

#endif // MUMBLE_MUMBLE_DISPATCHPROXYMODEL_H_
