## Curve Approximation by Iterative Transformation-Reparametrization technique
In many applications it is important to be able to approximate a given curve with a relatively low-order trigonometric polynomials.
Coefficients of such approximating polynomials may be used as features for building ML models (handwriting recognition application) or for data compression (electronic ink storage application).<br>
The quality of such approximation depends on <li>
order of polynomials;</li>
<li>parametrization of a curve.</li>
<br>For a given order of approximation, we want to find such parametrization of a given curve, so that the approximated curve will be closest to original in some norm.<br><br>
No known analytical solution exists for this problem, but there is a fast-converging iterative procedure that allows to generate parametrization with a very good approximation.<br><br>
[WebDemo](http://35.197.126.101/AppDemo/)
<br><br>OptiSpline library and API offers an implementation of this approach in C and C++.<br>
Sources are available, as well as builds for all major operating systems: Mac, Windows, Linux 








