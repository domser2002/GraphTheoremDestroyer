# Fact Tree logic documentation
## Known types of facts:
### general
1) IsConnectedFact (no parameters)
2) IsTreeFact (no parameters)
3) IstnaryTreeFact (one parameter - t)
4) IsPlanarFact (no parameters)
5) IsPartiteFact (one parameter - k, meaning k-partite graph)
6) IsCycleFact (no parameters)
7) IsCycleComplementFact (no parameters)
8) HasNoCyclesFact (no parameters)
### numeric
9) VertexCountFact (one parameter - v, function meaning |V(G)|)
10) MinVertexCountFact (one parameter - v, function meaning min |V(G)|)
11) MaxVertexCountFact (one parameter - v, function meaning max |V(G)|)
12) EdgeCountFact (one parameter - e, function meaning |E(G)|)
13) MinEdgeCountFact (one parameter - e, function meaning min |E(G)|)
14) MaxEdgeCountFact (one parameter - e, function meaning max |E(G)|)
15) TreeHeightFact (one parameter - h, function meaning height of tree, fact is valid only if IsTreeFact occurs)
16) MinTreeHeightFact (one parameter - h, function meaning min height of tree, fact is valid only if IsTreeFact occurs)
17) MaxTreeHeightFact (one parameter - h, function meaning max height of tree, fact is valid only if IsTreeFact occurs)
### structural
18) HasCycleFact (one parameter - k, meaning graph has C_k) 
19) HasNoCycleFact (one parameter - k, meaning graph has no C_k)
20) HasInducedCycleFact (one parameter - k, meaning graph has induced C_k)
21) HasNoInducedCycleFact (one parameter - k, meaning graph has no induced C_k)
22) HasMinorCycleFact (one parameter - k, meaning graph has C_k as minor)
23) HasNoMinorCycleFact (one parameter - k, meaning graph has no C_k as minor)
24) HasPathFact (one parameter - k, meaning graph has P_k) 
25) HasNoPathFact (one parameter - k, meaning graph has no P_k)
26) HasInducedPathFact (one parameter - k, meaning graph has induced P_k)
27) HasNoInducedPathFact (one parameter - k, meaning graph has no induced P_k)
28) HasMinorPathFact (one parameter - k, meaning graph has P_k as minor)
29) HasNoMinorPathFact (one parameter - k, meaning graph has no P_k as minor)
30) HasCompletePartiteFact (two parameters - k, meaning graph has complete k-partite graph and A - array of k partition class sizes)
31) HasNoCompletePartiteFact (two parameters - k, meaning graph has no complete k-partite graph and A - array of k partition class sizes)
32) HasInducedCompletePartiteFact (two parameters - k, meaning graph has induced complete k-partite graph and A - array of k partition class sizes)
33) HasNoInducedCompletePartiteFact (two parameters - k, meaning graph has no induced complete k-partite graph and A - array of k partition class sizes)
34) HasMinorCompletePartiteFact (two parameters - k, meaning graph has complete k-partite graph as minor and A - array of k partition class sizes)
35) HasNoMinorCompletePartiteFact (two parameters - k, meaning graph has no complete k-partite graph as minor and A - array of k partition class sizes)
36) HasCliqueFact (equivalent to induced) (one parameter - k, meaning graph has K_k)
37) HasNoCliqueFact (one parameter - k, meaning graph has no K_k)
38) HasMinorCliqueFact (one parameter - k, meaning graph has K_k as minor)
39) HasNoMinorCliqueFact (one parameter - k, meaning graph has no K_k as minor)

## Known contradictions:
1) MaxVertexCountFact (with v), MinEdgeCountFact (with e) contradict if (v*(v-1)) / 2 > e
2) MinEdgeCountFact (with e1), MaxEdgeCountFact (with e2) contradict if e1 > e2
3) MinVertexCountFact (with v1), MaxVertexCountFact (with v2) contradict if v1 > v2
4) MinEdgeCountFact (with e1), MaxEdgeCountFact (with e2), MinVertexCountFact (with v) contradict if e1, e2 and v are functions of |V(G)| and e1 > e2 for |V(G)| > v
5) IsPlanarFact, HasMinorCompletePartiteFact (with k and A) contradict if k=2, A >= {3,3}
6) IsPlanarFact, HasMinorCliqueFact (with k) contradict if k >= 5
7) HasCycleFact, HasNoCyclesFact contradict always
8) IsPlanarFact, MinEdgeCountFact (with e) contradict if e is a function of n (|V(G)|) and e > 3n - 6
9) IsPartiteFact (with k1), HasCycleFact (with k2) contradict if k1 = 2, k2 % 2 == 1

## Known implications:
1) EdgeCountFact (with e) implies MinEdgeCountFact (with e), MaxEdgeCountFact (with e)
2) VertexCountFact (with v) implies MinVertexCountFact (with v), MaxVertexCountFact (with v)
3) MinEdgeCountFact (with e) implies MinVertexCountFact (with v = ceil((1+sqrt(1+8*e))/2)) 
4) MaxVertexCountFact (with v) implies MaxEdgeCountFact (with e = v*(v-1) / 2)
5) IsTreeFact implies IsConnectedFact, HasNoCyclesFact
6) IsConnectedFact, HasNoCyclesFact implies IsTreeFact
7) IsPartiteFact (with k) implies MaxEdgeCountFact (with e = (n^2 * (k-1)^2)/k^2, n means |V(G)|)
8) IsTreeFact, HasNoInducedCompletePartiteFact (with r, A) implies IstnaryTreeFact (if r = 2 and A[0] = 1, with t = A[1] - 1)
9) IsTreeFact, HasNoInducedPathFact (with k) implies MaxTreeHeightFact (with h = k-1)
10) IstnaryTreeFact (with t), MaxTreeHeightFact (with h) implies MaxVertexCountFact (with v = (t^h - 1) / (t - 1))
11) HasNoMinorCompletePartiteFact (with k1 and A), HasNoMinorCliqueFact (with k2) implies IsPlanarFact (if k1 = 2, A <= {3,3}, k2 <= 5)
12) IsCycleComplementFact implies EdgeCountFact (with e = (n^2-3n)/2)
13) IsPlanarFact implies MaxEdgeCountFact (with e = 3n-6)
14) IsCycleComplementFact, MaxVertexCountFact (with v) implies IsPlanarFact (if v <= 6)
15) IsCycleComplementFact, VertexCountFact (with v) implies HasMinorCompletePartiteFact (if v = 7, with k = 2, A = {3,3})
16) IsCycleFact implies MinVertexCountFact (with v = 3), EdgeCountFact (with e = n, where n means |V(G)|)
17) IsTreeFact implies EdgeCountFact (with e = n - 1, where n means |V(G)|)
