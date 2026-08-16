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

#include "manager.hpp"

Manager::~Manager()
{
    qDeleteAll(mPlayers);
}

Player *Manager::queue(const QString &filename, float volume)
{
    Player *player;

    if (mIdlePlayers.isEmpty()) {
        player = new Player;
        connect(player, &Player::finished, this, &Manager::onPlayerFinished);
        connect(player, &Player::idleTimeout, this, &Manager::onPlayerIdleTimeout);
        mPlayers.append(player);
    } else {
        player = mIdlePlayers.takeFirst();
    }

    player->init(filename, volume);
    return player;
}

void Manager::stopAll()
{
    for (auto i : std::as_const(mPlayers)) {
        i->stop();
    }
}

void Manager::onPlayerFinished()
{
    Player *player = dynamic_cast<Player *>(sender());
    mIdlePlayers.append(player);
}

void Manager::onPlayerIdleTimeout()
{
    Player *player = dynamic_cast<Player *>(sender());
    mIdlePlayers.removeOne(player);
    mPlayers.removeOne(player);
    player->deleteLater();
}