#include <youth/core/DateTime.hpp>

#include <gtest/gtest.h>

#include <iostream>

using namespace youth::core;

DateTime getDateTime()
{
    auto dateTime = DateTime::currentDateTime();
    std::cout << dateTime.toStandardFormat() << std::endl;

    dateTime = DateTime::fromString("2020-01-31 08:44:03", "%Y-%m-%d %H:%M:%S");
    EXPECT_EQ(dateTime.toStandardFormat(), "2020-01-31 08:44:03.000000");
    return dateTime;
}

TEST(addMicroSecondsTest, Positive)
{
    auto dateTime = getDateTime();

    auto addMicroSeconds = dateTime.addMicroSeconds(1);
    EXPECT_EQ(addMicroSeconds.toStandardFormat(), "2020-01-31 08:44:03.000001");
    EXPECT_FALSE(dateTime > addMicroSeconds);
    addMicroSeconds = dateTime.addMicroSeconds(-1);
    EXPECT_EQ(addMicroSeconds.toStandardFormat(), "2020-01-31 08:44:02.999999");
    EXPECT_TRUE(dateTime > addMicroSeconds);
}

TEST(addMilliSecondsTest, Positive)
{
    auto dateTime = getDateTime();

    auto addMilliSeconds = dateTime.addMilliSeconds(1);
    EXPECT_EQ(addMilliSeconds.toStandardFormat(), "2020-01-31 08:44:03.001000");
    EXPECT_FALSE(dateTime > addMilliSeconds);
    addMilliSeconds = dateTime.addMilliSeconds(-1);
    EXPECT_EQ(addMilliSeconds.toStandardFormat(), "2020-01-31 08:44:02.999000");
    EXPECT_TRUE(dateTime > addMilliSeconds);
}

TEST(addSecondsTest, Positive)
{
    auto dateTime = getDateTime();

    auto addSeconds = dateTime.addSeconds(1);
    EXPECT_EQ(addSeconds.toStandardFormat(), "2020-01-31 08:44:04.000000");
    EXPECT_FALSE(dateTime > addSeconds);
    addSeconds = dateTime.addSeconds(-1);
    EXPECT_EQ(addSeconds.toStandardFormat(), "2020-01-31 08:44:02.000000");
    EXPECT_TRUE(dateTime > addSeconds);
}

TEST(addMinutesTest, Positive)
{
    auto dateTime = getDateTime();

    auto addMinutes = dateTime.addMinutes(1);
    EXPECT_EQ(addMinutes.toStandardFormat(), "2020-01-31 08:45:03.000000");
    EXPECT_FALSE(dateTime > addMinutes);
    addMinutes = dateTime.addMinutes(-1);
    EXPECT_EQ(addMinutes.toStandardFormat(), "2020-01-31 08:43:03.000000");
    EXPECT_TRUE(dateTime > addMinutes);
}

TEST(addHoursTest, Positive)
{
    auto dateTime = getDateTime();

    auto addHours = dateTime.addHours(1);
    EXPECT_EQ(addHours.toStandardFormat(), "2020-01-31 09:44:03.000000");
    EXPECT_FALSE(dateTime > addHours);
    addHours = dateTime.addHours(-1);
    EXPECT_EQ(addHours.toStandardFormat(), "2020-01-31 07:44:03.000000");
    EXPECT_TRUE(dateTime > addHours);
}

TEST(addDaysTest, Positive)
{
    auto dateTime = getDateTime();

    auto addDays = dateTime.addDays(1);
    EXPECT_EQ(addDays.toStandardFormat(), "2020-02-01 08:44:03.000000");
    EXPECT_FALSE(dateTime > addDays);
    addDays = dateTime.addDays(-1);
    EXPECT_EQ(addDays.toStandardFormat(), "2020-01-30 08:44:03.000000");
    EXPECT_TRUE(dateTime > addDays);
}

TEST(addWeeksTest, Positive)
{
    auto dateTime = getDateTime();

    auto addWeeks = dateTime.addWeeks(1);
    EXPECT_EQ(addWeeks.toStandardFormat(), "2020-02-07 08:44:03.000000");
    EXPECT_FALSE(dateTime > addWeeks);
    addWeeks = dateTime.addWeeks(-1);
    EXPECT_EQ(addWeeks.toStandardFormat(), "2020-01-24 08:44:03.000000");
    EXPECT_TRUE(dateTime > addWeeks);
}

TEST(addMonthsTest, Positive)
{
    auto dateTime = DateTime::fromString("2022-08-26 08:44:03", "%Y-%m-%d %H:%M:%S");

    auto addMonths = dateTime.addMonths(1);
    EXPECT_EQ(addMonths.toStandardFormat(), "2022-09-26 08:44:03.000000");
    EXPECT_FALSE(dateTime > addMonths);
    addMonths = dateTime.addMonths(-1);
    EXPECT_EQ(addMonths.toStandardFormat(), "2022-07-26 08:44:03.000000");
    EXPECT_TRUE(dateTime > addMonths);

    dateTime = DateTime::fromString("2021-01-31 08:44:03", "%Y-%m-%d %H:%M:%S");
    addMonths = dateTime.addMonths(1);
    EXPECT_EQ(addMonths.toStandardFormat(), "2021-02-28 08:44:03.000000");

    dateTime = DateTime::fromString("2021-03-31 08:44:03", "%Y-%m-%d %H:%M:%S");
    addMonths = dateTime.addMonths(-1);
    EXPECT_EQ(addMonths.toStandardFormat(), "2021-02-28 08:44:03.000000");

    dateTime = DateTime::fromString("2020-01-31 08:44:03", "%Y-%m-%d %H:%M:%S");
    addMonths = dateTime.addMonths(1);
    EXPECT_EQ(addMonths.toStandardFormat(), "2020-02-29 08:44:03.000000");

    dateTime = DateTime::fromString("2020-03-31 08:44:03", "%Y-%m-%d %H:%M:%S");
    addMonths = dateTime.addMonths(-1);
    EXPECT_EQ(addMonths.toStandardFormat(), "2020-02-29 08:44:03.000000");
}

TEST(addYearsTest, Positive)
{
    auto dateTime = DateTime::fromString("2022-08-26 08:44:03", "%Y-%m-%d %H:%M:%S");

    auto addYears = dateTime.addYears(1);
    EXPECT_EQ(addYears.toStandardFormat(), "2023-08-26 08:44:03.000000");
    EXPECT_FALSE(dateTime > addYears);
    addYears = dateTime.addYears(-1);
    EXPECT_EQ(addYears.toStandardFormat(), "2021-08-26 08:44:03.000000");
    EXPECT_TRUE(dateTime > addYears);

    dateTime = DateTime::fromString("2020-02-29 08:44:03", "%Y-%m-%d %H:%M:%S");
    addYears = dateTime.addYears(1);
    EXPECT_EQ(addYears.toStandardFormat(), "2021-02-28 08:44:03.000000");

    dateTime = DateTime::fromString("2020-02-29 08:44:03", "%Y-%m-%d %H:%M:%S");
    addYears = dateTime.addYears(-1);
    EXPECT_EQ(addYears.toStandardFormat(), "2019-02-28 08:44:03.000000");
}
