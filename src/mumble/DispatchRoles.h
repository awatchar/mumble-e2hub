// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_DISPATCHROLES_H_
#define MUMBLE_MUMBLE_DISPATCHROLES_H_

#include <QtCore/Qt>

namespace DispatchRoles {
enum Role {
	DisplayName = Qt::UserRole + 1,
	ChannelName,
	SessionId,
	TalkingState,
	Muted,
	Deafened,
	LocalMuted
};
} // namespace DispatchRoles

#endif // MUMBLE_MUMBLE_DISPATCHROLES_H_
