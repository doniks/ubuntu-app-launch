/*
 * Copyright © 2017 Canonical Ltd.
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

#include "jobs-systemd.h"

#include "eventually-fixture.h"
#include "registry-mock.h"
#include "systemd-mock.h"

class JobsSystemd : public EventuallyFixture
{
protected:
    std::shared_ptr<DbusTestService> service;
    std::shared_ptr<RegistryMock> registry;
    std::shared_ptr<SystemdMock> systemd;
    GDBusConnection* bus = nullptr;

    virtual void SetUp()
    {
        /* Get the applications dir */
        g_setenv("XDG_DATA_DIRS", CMAKE_SOURCE_DIR, TRUE);

        /* Force over to session bus */
        g_setenv("UBUNTU_APP_LAUNCH_SYSTEMD_PATH", "/this/should/not/exist", TRUE);

        service = std::shared_ptr<DbusTestService>(dbus_test_service_new(nullptr),
                                                   [](DbusTestService* service) { g_clear_object(&service); });

        systemd = std::make_shared<SystemdMock>(
            std::list<SystemdMock::Instance>{{defaultJobName(), std::string{simpleAppID()}, "1234567890"},
                                             {defaultJobName(), std::string{simpleAppID()}, "0987654321"}});
        dbus_test_service_add_task(service.get(), *systemd);

        dbus_test_service_start_tasks(service.get());
        registry = std::make_shared<RegistryMock>();

        bus = g_bus_get_sync(G_BUS_TYPE_SESSION, nullptr, nullptr);
        g_dbus_connection_set_exit_on_close(bus, FALSE);
        g_object_add_weak_pointer(G_OBJECT(bus), (gpointer*)&bus);
    }

    virtual void TearDown()
    {
        systemd.reset();
        registry.reset();
        service.reset();

        g_object_unref(bus);
        ASSERT_EVENTUALLY_EQ(nullptr, bus);
    }

    std::string defaultJobName()
    {
        return "application-legacy";
    }

    ubuntu::app_launch::AppID simpleAppID()
    {
        return {ubuntu::app_launch::AppID::Package::from_raw({}),
                ubuntu::app_launch::AppID::AppName::from_raw("single"),
                ubuntu::app_launch::AppID::Version::from_raw({})};
    }
};

/* Make sure we can build an object and destroy it */
TEST_F(JobsSystemd, Init)
{
    registry->impl->jobs = std::make_shared<ubuntu::app_launch::jobs::manager::SystemD>(registry);
}

/* Make sure we make the initial call to get signals and an initial list */
TEST_F(JobsSystemd, Startup)
{
    registry->impl->jobs = std::make_shared<ubuntu::app_launch::jobs::manager::SystemD>(registry);

    EXPECT_EVENTUALLY_FUNC_EQ(true, std::function<bool()>([this]() { return systemd->subscribeCallsCnt() > 0; }));
    EXPECT_EVENTUALLY_FUNC_EQ(true, std::function<bool()>([this]() -> bool { return systemd->listCallsCnt() > 0; }));
}

TEST_F(JobsSystemd, RunningApps)
{
    auto manager = std::make_shared<ubuntu::app_launch::jobs::manager::SystemD>(registry);
    registry->impl->jobs = manager;

    auto apps = manager->runningApps();
    ASSERT_FALSE(apps.empty());
    EXPECT_EQ(1u, apps.size());

    auto app = *apps.begin();

    EXPECT_EQ(simpleAppID(), app->appId());

    auto instances = app->instances();

    ASSERT_FALSE(instances.empty());
    EXPECT_EQ(2u, instances.size());
}
