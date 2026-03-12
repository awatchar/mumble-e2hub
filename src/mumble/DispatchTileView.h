// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_DISPATCHTILEVIEW_H_
#define MUMBLE_MUMBLE_DISPATCHTILEVIEW_H_

#include <QtWidgets/QListView>

class DispatchTileView : public QListView {
public:
	enum class TileSizePreset {
		Normal,
		Large
	};

private:
	Q_OBJECT
	Q_DISABLE_COPY(DispatchTileView)

public:
	explicit DispatchTileView(QWidget *parent = nullptr);

	void setTileSizePreset(TileSizePreset preset);
	TileSizePreset tileSizePreset() const;
	~DispatchTileView() Q_DECL_OVERRIDE;

private:
	TileSizePreset m_tileSizePreset = TileSizePreset::Normal;
};

#endif // MUMBLE_MUMBLE_DISPATCHTILEVIEW_H_
