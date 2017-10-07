#include <iostream>
#include <vector>
#include <utility>
#include <cmath>
#include <random>
#include <chrono>
#include <algorithm>
#include <iterator>
 
using namespace std;

typedef pair<int, int> point_t;
typedef pair<point_t, point_t> points_t;
 
int distance_between(const point_t& a, const point_t& b) {
	return pow(b.first - a.first, 2) + pow(b.second - a.second, 2);
}
 
pair<int, points_t> find_closest_brute(const vector<point_t>& points) {
	if (points.size() < 2) {
		return { 0, { { 0, 0 }, { 0, 0 } } };
	}
	auto minDistance = abs(distance_between(points.at(0), points.at(1)));
	points_t minPoints = { points.at(0), points.at(1) };
	for (auto i = begin(points); i != (end(points) - 1); ++i) {
		for (auto j = i + 1; j < end(points); ++j) {
			auto newDistance = abs(distance_between(*i, *j));
			if (newDistance < minDistance) {
				minDistance = newDistance;
				minPoints.first = *i;
				minPoints.second = *j;
			}
		}
	}
	return { minDistance, minPoints };
}
 
pair<int, points_t> find_closest_optimized(const vector<point_t>& xP,
	const vector<point_t>& yP) {
	if (xP.size() <= 3) {
		return find_closest_brute(xP);
	}
	auto N = xP.size();
	auto xL = vector<point_t>();
	auto xR = vector<point_t>();
	copy(begin(xP), begin(xP) + (N / 2), back_inserter(xL));
	copy(begin(xP) + (N / 2), end(xP), back_inserter(xR));
	auto xM = xP.at(N / 2).first;
	auto yL = vector<point_t>();
	auto yR = vector<point_t>();
	copy_if(begin(yP), end(yP), back_inserter(yL), [&xM](const point_t& p) {
		return p.first <= xM;
	});
	copy_if(begin(yP), end(yP), back_inserter(yR), [&xM](const point_t& p) {
		return p.first > xM;
	});
	auto p1 = find_closest_optimized(xL, yL);
	auto p2 = find_closest_optimized(xR, yR);
	auto minPair = (p1.first <= p2.first) ? p1 : p2;
	auto yS = vector<point_t>();
	copy_if(begin(yP), end(yP), back_inserter(yS), [&minPair, &xM](const point_t& p) {
		return abs(xM - p.first) < minPair.first;
	});
	auto result = minPair;
	for (auto i = begin(yS); i != (end(yS) - 1); ++i) {
		for (auto k = i + 1; k != end(yS) &&
		 ((k->second - i->second) < minPair.first); ++k) {
			auto newDistance = abs(distance_between(*k, *i));
			if (newDistance < result.first) {
				result = { newDistance, { *k, *i } };
			}
		}
	}
	return result;
}

 
int main(int argc, char * argv[]) {
	// default_random_engine re(chrono::system_clock::to_time_t(
	// 	chrono::system_clock::now()));
	// uniform_real_distribution<int> urd(-500.0, 500.0);
	// vector<point_t> points(100);
	// generate(begin(points), end(points), [&urd, &re]() {
 //                return point_t { 1000 + urd(re), 1000 + urd(re) };
 //        });
	// auto answer = find_closest_brute(points);
	// sort(begin(points), end(points), [](const point_t& a, const point_t& b) {
	// 	return a.first < b.first;
	// });
	// auto xP = points;
	// sort(begin(points), end(points), [](const point_t& a, const point_t& b) {
	// 	return a.second < b.second;
	// });
	// auto yP = points;
	// cout << "Min distance (brute): " << answer.first << " ";
	// print_point(answer.second.first);
	// cout << ", ";
	// print_point(answer.second.second);
	// answer = find_closest_optimized(xP, yP);
	// cout << "\nMin distance (optimized): " << answer.first << " ";
	// print_point(answer.second.first);
	// cout << ", ";
	// print_point(answer.second.second);
	int N;
	cin >> N;
	vector<point_t> points(N);
	int x, y;
	for(int i=0; i<N; i++)
	{
		cin >> x >> y;
		points[i].first = abs(x);
		points[i].second = abs(y);
	}
	sort(begin(points), end(points), [](const point_t& a, const point_t& b) {
		return a.first < b.first;
	});
	auto xP = points;
	sort(begin(points), end(points), [](const point_t& a, const point_t& b) {
		return a.second < b.second;
	});
	auto yP = points;
	auto ans = find_closest_optimized(xP, yP);
	cout << ans.first << endl;
	return 0;
}