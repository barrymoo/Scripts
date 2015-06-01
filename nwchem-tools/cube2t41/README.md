cube2t41
========

This tool is used to convert a set of gaussian `cube` files to human readable `TAPE41`
[Amsterdam Density Functional](www.scm.com) format. You will need access to the the `udmpkf` script
from `ADF` for this script to be useful. Steps to generate `*.t41` file:

<ol>
  <li> Generate <code>cube</code> files. </li>
  <li><code>cubefilelist.py</code> to generate file <code>cubefiles</code></li>
  <li><code>cub2t41</code> to generate <code>ascii.t41</code></li>
  <li><code>udmpkf < ascii.t41 bin.t41</code></li>
  <li><code>adfview bin.t41</code></li>
</ol>
