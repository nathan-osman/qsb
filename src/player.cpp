/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2026 Nathan Osman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "player.hpp"

Player::Player()
{
    mMediaPlayer.setAudioOutput(&mAudioOutput);
    connect(&mMediaPlayer, &QMediaPlayer::errorOccurred, this, &Player::error);
    connect(&mMediaPlayer, &QMediaPlayer::playbackStateChanged, this, &Player::onPlaybackStateChanged);

    mTimer.setInterval(30000);
    mTimer.setSingleShot(true);
    connect(&mTimer, &QTimer::timeout, this, &Player::idleTimeout);
}

void Player::play(const QString &filename, float volume)
{
    mTimer.stop();
    mAudioOutput.setVolume(volume);
    mMediaPlayer.setSource(filename);
    mMediaPlayer.play();
}

void Player::stop()
{
    mMediaPlayer.stop();
    mTimer.start();
}

void Player::onPlaybackStateChanged(QMediaPlayer::PlaybackState state)
{
    if (state == QMediaPlayer::StoppedState) {
        emit finished();
    }
}