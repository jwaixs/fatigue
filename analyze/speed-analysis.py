import pandas

import numpy as np

from matplotlib import pyplot as plt
from datetime import datetime

df = pandas.read_csv('./res_per_question.csv')
time_per_question = df['time_to_solve']

time_per_question_histogram, bins = np.histogram(
    time_per_question,
    bins = 'auto'
)

center = (bins[:-1] + bins[1:]) / 2
width = 0.8 * (bins[1] - bins[0])
plt.bar(center, time_per_question_histogram, align='center', width=width)
plt.show()

def is_date_in_hour(date, hour):
    dt = datetime.strptime(date, '%Y-%m-%d %H:%M:%S')
    return dt.hour == hour

hours = range(0, 24)
speed_results_per_hour = list()
for hour in hours:
    hour_results = df[df['date'].map(lambda x : is_date_in_hour(x, hour))]

    median = hour_results['time_to_solve'].median()
    smin = hour_results['time_to_solve'].min()
    smax = hour_results['time_to_solve'].max()
    mean = hour_results['time_to_solve'].mean()
    std = hour_results['time_to_solve'].std()
    num = len(hour_results['time_to_solve'])

    if num > 50:
        speed_results_per_hour.append((median, mean, mean + std, mean - std, smin, smax, num))
    else:
        speed_results_per_hour.append((np.nan, np.nan))

print(speed_results_per_hour)

plt.plot(hours, map(lambda x : x[0], speed_results_per_hour))
plt.show()

def is_date_in_week_day(date, week_day):
    dt = datetime.strptime(date, '%Y-%m-%d %H:%M:%S')
    return dt.weekday() == week_day

days = range(7)
speed_results_per_day = list()
for day in days:
    day_results = df[df['date'].map(lambda x : is_date_in_week_day(x, day))]

    median = day_results['time_to_solve'].median()
    smin = day_results['time_to_solve'].min()
    smax = day_results['time_to_solve'].max()
    mean = day_results['time_to_solve'].mean()
    std = day_results['time_to_solve'].std()
    num = len(day_results['time_to_solve'])

    if num > 50:
        speed_results_per_day.append((median, mean, mean + std, mean - std, smin, smax, num))
    else:
        speed_results_per_day.append((np.nan, np.nan))

plt.plot(days, map(lambda x : x[0], speed_results_per_day))
plt.show()

