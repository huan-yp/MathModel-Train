import solve_args as sa
import array
ans = []
for month in range(1, 13):
    Sum = []
    for year in range(2000, 2022):
        data = sa.find_data(year, month, "Lake Ontario")
        if data is None:
            continue
        else:
            Sum.append(data)
    Sum = sorted(Sum)
    if len(Sum) >= 11:
        if 1 <= month <= 6: 
            ans.append(0.32 * sum(Sum[-6:]) + 0.11 * sum(Sum[-2:]) + 0.54 * sum(Sum[-11:]) + 0.03 * Sum[0])
        else:
            ans.append(0.32 * sum(Sum[-6:]) + 0.11 * sum(Sum[-2:]) + 0.54 * sum(Sum[:10]) + 0.03 * Sum[0])
formatted_ans = ["{:.2f}".format(val) for val in ans if val is not None]
print(formatted_ans)
#print("{:.2f}".format(ans))
