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

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QAction>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QWidget>

#include "category.hpp"
#include "manager.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow();

private slots:

    void onNew();
    void onOpen();
    void onSave();
    void onSaveAs();
    void onAbout();

    void onCategoryRemove();

    void onNewCategory();

    void onMarkDirty();

protected:

    void closeEvent(QCloseEvent *event);

private:

    void initUi();
    void initDocument();

    bool loadDocument(const QString &filename);
    bool saveDocument(const QString &filename);

    Category *newCategory();

    bool cannotClose();
    void setFilename(const QString &);

    Manager mManager;

    QAction mActionSave;
    QHBoxLayout mLayoutMain;
    QWidget *mWidgetCategories;
    QHBoxLayout *mLayoutCategories;

    QString mFilename;
};

#endif // MAINWINDOW_HPP