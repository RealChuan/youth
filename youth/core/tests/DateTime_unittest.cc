#include <youth/core/DateTime.hpp>

#include <iostream>

using namespace youth::core;

void addTest()
{
    auto dateTime = DateTime::currentDateTime();
    std::cout << dateTime.toStandardFormat() << std::endl;

    std::cout << "add micro seconds--------------------" << std::endl;
    auto addMicroSeconds = dateTime.addMicroSeconds(1);
    std::cout << addMicroSeconds.toStandardFormat() << std::endl;
    std::cout << (addMicroSeconds > dateTime) << std::endl;
    addMicroSeconds = dateTime.addMicroSeconds(-1);
    std::cout << addMicroSeconds.toStandardFormat() << std::endl;
    std::cout << (addMicroSeconds > dateTime) << std::endl;

    std::cout << "add milli seconds--------------------" << std::endl;
    auto addMilliSeconds = dateTime.addMilliSeconds(1000);
    std::cout << addMilliSeconds.toStandardFormat() << std::endl;
    std::cout << (addMilliSeconds > dateTime) << std::endl;
    addMilliSeconds = dateTime.addMilliSeconds(-1000);
    std::cout << addMilliSeconds.toStandardFormat() << std::endl;
    std::cout << (addMilliSeconds > dateTime) << std::endl;

    std::cout << "add seconds--------------------" << std::endl;
    auto addSeconds = dateTime.addSeconds(1);
    std::cout << addSeconds.toStandardFormat() << std::endl;
    std::cout << (addSeconds > dateTime) << std::endl;
    addSeconds = dateTime.addSeconds(-1);
    std::cout << addSeconds.toStandardFormat() << std::endl;
    std::cout << (addSeconds > dateTime) << std::endl;

    std::cout << "add minutes---------------------" << std::endl;
    auto addMinutes = dateTime.addMinutes(1);
    std::cout << addMinutes.toStandardFormat() << std::endl;
    std::cout << (addMinutes > dateTime) << std::endl;
    addMinutes = dateTime.addMinutes(-1);
    std::cout << addMinutes.toStandardFormat() << std::endl;
    std::cout << (addMinutes > dateTime) << std::endl;

    std::cout << "add hours--------------------" << std::endl;
    auto addHours = dateTime.addHours(1);
    std::cout << addHours.toStandardFormat() << std::endl;
    std::cout << (addHours > dateTime) << std::endl;
    addHours = dateTime.addHours(-1);
    std::cout << addHours.toStandardFormat() << std::endl;
    std::cout << (addHours > dateTime) << std::endl;

    std::cout << "add days--------------------" << std::endl;
    auto addDays = dateTime.addDays(1);
    std::cout << addDays.toStandardFormat() << std::endl;
    std::cout << (addDays > dateTime) << std::endl;
    addDays = dateTime.addDays(-1);
    std::cout << addDays.toStandardFormat() << std::endl;
    std::cout << (addDays > dateTime) << std::endl;

    std::cout << "add weeks--------------------" << std::endl;
    auto addWeeks = dateTime.addWeeks(1);
    std::cout << addWeeks.toStandardFormat() << std::endl;
    std::cout << (addWeeks > dateTime) << std::endl;
    addWeeks = dateTime.addWeeks(-1);
    std::cout << addWeeks.toStandardFormat() << std::endl;
    std::cout << (addWeeks > dateTime) << std::endl;

    std::cout << "add months--------------------" << std::endl;
    auto addMonths = dateTime.addMonths(10);
    std::cout << addMonths.toStandardFormat() << std::endl;
    std::cout << (addMonths > dateTime) << std::endl;
    addMonths = dateTime.addMonths(-10);
    std::cout << addMonths.toStandardFormat() << std::endl;
    std::cout << (addMonths > dateTime) << std::endl;

    std::cout << "add years---------------------" << std::endl;
    auto addYears = dateTime.addYears(10);
    std::cout << addYears.toStandardFormat() << std::endl;
    std::cout << (addYears > dateTime) << std::endl;
    addYears = dateTime.addYears(-10);
    std::cout << addYears.toStandardFormat() << std::endl;
    std::cout << (addYears > dateTime) << std::endl;
}

void addFebruary29thTest()
{
    std::cout << "form String--------------------" << std::endl;
    auto formString = DateTime::fromString("2020-01-31 08:44:03", "%Y-%m-%d %H:%M:%S");
    formString = formString.addMicroSeconds(123456);
    std::cout << formString.toStandardFormat() << std::endl;

    std::cout << "add February Month--------------------" << std::endl;
    auto addFebruaryMouths = formString.addMonths(1);
    std::cout << addFebruaryMouths.toStandardFormat() << std::endl;

    std::cout << "add February Year--------------------" << std::endl;
    auto addFebruaryYears = addFebruaryMouths.addYears(1);
    std::cout << addFebruaryYears.toStandardFormat() << std::endl;
}

void addFebruary29thTest2()
{
    std::cout << "form String-2-------------------" << std::endl;
    auto formString = DateTime::fromString("2022-01-31 08:44:03", "%Y-%m-%d %H:%M:%S");
    formString = formString.addMicroSeconds(123456);
    std::cout << formString.toStandardFormat() << std::endl;

    std::cout << "add February Month-2-------------------" << std::endl;
    auto addFebruaryMouths = formString.addMonths(1);
    std::cout << addFebruaryMouths.toStandardFormat() << std::endl;

    std::cout << "add February Year-2-------------------" << std::endl;
    auto addFebruaryYears = addFebruaryMouths.addYears(1);
    std::cout << addFebruaryYears.toStandardFormat() << std::endl;
}

int main()
{
    addTest();

    addFebruary29thTest();

    addFebruary29thTest2();

    return 0;
}