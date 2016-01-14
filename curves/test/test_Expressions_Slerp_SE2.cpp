/*
 * @file test_Expressions_Slerp_SE2.cpp
 * @date Nov 25, 2015
 * @author Renaud Dubé, Abel Gawel
 */

#include <gtest/gtest.h>
#include <curves/SlerpSE2Curve.hpp>
#include <curves/Pose2_Expressions.hpp>
#include "gtsam/nonlinear/Expression.h"
#include <gtsam/nonlinear/NonlinearFactorGraph.h>
#include <gtsam/nonlinear/LevenbergMarquardtOptimizer.h>
#include "gtsam/nonlinear/ExpressionFactor.h"
#include <gtsam/base/numericalDerivative.h>

#include <Eigen/Core>
#include <boost/assign/list_of.hpp>

#include <math.h>

#include <kindr/minimal/testing-gtsam.h>

#include <curves/KeyGenerator.hpp>

#define DIM 3

using namespace curves;
using namespace gtsam;
using namespace std;

typedef typename SlerpSE2Curve::ValueType ValueType;
typedef Pose2 SE2;
typedef Rot2 SO2;

const double tolerance = 1e-3;
const double fd_step = 1e-9;

// Check for consistency between curve.evaluate() and curve.getValueExpression()
TEST(CurvesTestSuite, testSlerpSE2EvaluationVSExpression) {
  SlerpSE2Curve curve;
  const double t[] = {0, 10};
  const double evalTime = 5;

  // setup two SE2 objects
  ValueType poseA(0,0,0);
  ValueType poseB(2,2,M_PI/8);

  std::vector<Time> times(t,t+2);
  std::vector<ValueType> values;
  values.push_back(poseA);
  values.push_back(poseB);

  // interpolate curve
  curve.fitCurve(times, values);

  // get expression at evaluation time
  Expression<ValueType> expression = curve.getValueExpression(evalTime);

  // fill retrieved coefficients in gtsam values container
  Values gtsamValues;
  curve.initializeGTSAMValues(&gtsamValues);

  // read out SE2 object from values container
  ValueType result = expression.value(gtsamValues);

  SE2 directResult = curve.evaluate(evalTime);

  // assert return values are as expected
  ASSERT_EQ(directResult.x(), result.x());
  ASSERT_EQ(directResult.y(), result.y());
  ASSERT_EQ(directResult.theta(), result.theta());
}
//
//TEST(CurvesTestSuite, testRelativeMeasurementExpression) {
//
//  const double t[] = {0, 10, 20, 30};
//  std::vector<Time> times(t,t+4);
//
//  for (int i = 0; i < N_TEST_ITERATIONS; ++i) {
//    // setup four SE2 objects
//    std::vector<ValueType> values;
//    values.push_back(makeRandomSE2());
//    values.push_back(makeRandomSE2());
//    values.push_back(makeRandomSE2());
//    values.push_back(makeRandomSE2());
//
//    // interpolate curve
//    SlerpSE2Curve curve;
//    curve.fitCurve(times, values);
//
//    Values gtsamValues;
//    curve.initializeGTSAMValues(&gtsamValues);
//
//    Time ta = ((double)rand() / RAND_MAX) * 30;
//    Time tb = ((double)rand() / RAND_MAX) * 30;
//
//    Expression<SE2> TA(curve.getValueExpression(ta));
//    Expression<SE2> TB(curve.getValueExpression(tb));
//    Expression<SE2> predicted = compose(inverse(TA),TB);
//
//    testExpressionJacobians(predicted, gtsamValues, fd_step, tolerance);
//  }
//}
//
//TEST(CurvesTestSuite, testLandmarkDistanceExpression) {
//
//  const double t[] = {0, 10, 20, 30};
//  std::vector<Time> times(t,t+4);
//
//  for (int i = 0; i < N_TEST_ITERATIONS; ++i) {
//    // setup four SE2 objects
//    std::vector<ValueType> values;
//    values.push_back(makeRandomSE2());
//    values.push_back(makeRandomSE2());
//    values.push_back(makeRandomSE2());
//    values.push_back(makeRandomSE2());
//
//    // interpolate curve
//    SlerpSE2Curve curve;
//    curve.fitCurve(times, values);
//
//    Values gtsamValues;
//    curve.initializeGTSAMValues(&gtsamValues);
//
//    Vector2 v;
//    v.setRandom();
//    Point2 Pobservation(v);
//    v.setRandom();
//    Point2 Plandmark(v);
//
//    Key lmKey = KeyGenerator::getNextKey();
//    gtsamValues.insert(lmKey,Plandmark);
//
//    Time t = ((double)rand() / RAND_MAX) * 30;
//
//    Expression<Point2> EpObservation(Pobservation);
//    Expression<Point2> EpLandmark(lmKey);
//
//    Expression<SE2> Epose = curve.getValueExpression(t);
//
//    Expression<Point2> EpObservationInWorld = transformFromCurve(Epose, EpObservation);
//    Expression<double> Edistance = distanceBetweenPoints(EpLandmark, EpObservationInWorld);
//
//    testExpressionJacobians(Edistance, gtsamValues, fd_step, tolerance);
//  }
//}


