# Benchmark Results
| Dataset|Method|IdentitySwitches|Fragmentations|AverageTrackContinuity|IdentityPreservationRate|TracksCreated|TracksDeleted|SuccessfulAssociations|MissedAssociations|AveragePredictionError|MaximumPredictionError|TotalAssociationRuntimeMs|AverageAssociationRuntimeMs |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| false_detection_demo|Hungarian|8|3|7.33333|62.2642|7|0|49|0|31.7335|139.628|0.220625|0.0245139 |
| false_detection_demo|Greedy|0|0|8.83333|100|9|2|47|0|4.52031|21.2603|0.102042|0.011338 |
| occlusion_demo|Hungarian|0|1|8.5|95|4|0|34|0|3.31426|20.3961|0.282626|0.0314029 |
| occlusion_demo|Greedy|0|1|8.5|95|4|0|34|0|3.31426|20.3961|0.070791|0.00786567 |
| curved_demo|Hungarian|0|0|10|100|4|0|36|0|3.97052|20.3961|0.092084|0.0102316 |
| curved_demo|Greedy|0|0|10|100|4|0|36|0|3.97052|20.3961|0.066917|0.00743522 |
| crossing_demo|Hungarian|0|0|10|100|4|0|36|0|3.12809|20.3961|0.088417|0.00982411 |
| crossing_demo|Greedy|0|0|10|100|4|0|36|0|3.12809|20.3961|0.128498|0.0142776 |
| prediction_demo_clean|Hungarian|0|0|10|100|4|0|36|0|3.12809|20.3961|0.142667|0.0158519 |
| prediction_demo_clean|Greedy|0|0|10|100|4|0|36|0|3.12809|20.3961|0.083042|0.00922689 |
| prediction_demo_noisy|Hungarian|0|0|10|100|4|0|36|0|12.0591|33|0.096541|0.0107268 |
| prediction_demo_noisy|Greedy|1|0|10|77.5|5|1|35|0|12.4003|29.8296|0.081417|0.00904633 |
| acceleration_demo|Hungarian|0|0|10|100|4|0|36|0|3.92812|20.3961|0.103835|0.0115372 |
| acceleration_demo|Greedy|0|0|10|100|4|0|36|0|3.92812|20.3961|0.072416|0.00804622 |
