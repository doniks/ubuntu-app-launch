/*
 * Copyright © 2016 Canonical Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3, as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranties of
 * MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors:
 *     Ted Gould <ted.gould@canonical.com>
 */

#include "application-impl-base.h"
#include "application-info-desktop.h"
#include "snapd-info.h"

#pragma once

namespace ubuntu
{
namespace app_launch
{
namespace app_impls
{

class Snap : public Base
{
public:
    Snap(const AppID& appid, const std::shared_ptr<Registry>& registry);
    Snap(const AppID& appid, const std::shared_ptr<Registry>& registry, const std::string& interface);

    static std::list<std::shared_ptr<Application>> list(const std::shared_ptr<Registry>& registry);

    AppID appId() override;

    std::shared_ptr<Info> info() override;

    std::vector<std::shared_ptr<Instance>> instances() override;

    std::shared_ptr<Instance> launch(const std::vector<Application::URL>& urls = {}) override;
    std::shared_ptr<Instance> launchTest(const std::vector<Application::URL>& urls = {}) override;

    static bool hasAppId(const AppID& appId, const std::shared_ptr<Registry>& registry);

    static bool verifyPackage(const AppID::Package& package);
    static bool verifyAppname(const AppID::Package& package, const AppID::AppName& appname);
    static AppID::AppName findAppname(const AppID::Package& package, AppID::ApplicationWildcard card);
    static AppID::Version findVersion(const AppID::Package& package, const AppID::AppName& appname);

private:
    AppID appid_;
    std::shared_ptr<app_info::Desktop> info_;
    std::string interface_;
    std::shared_ptr<snapd::Info::PkgInfo> pkgInfo_;

    std::list<std::pair<std::string, std::string>> launchEnv();
    static std::string findInterface(const AppID& appid, const std::shared_ptr<Registry>& registry);
};

}  // namespace app_impls
}  // namespace app_launch
}  // namespace ubuntu
