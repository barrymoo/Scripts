spec-gen.py
===========

I wrote this spectral broadening tool because I had some trouble understanding
the units in the previous tool I used.

What types of spectra can `spec-gen.py` handle?

* Absorption and Circular Dichroism
* Infrared and Vibrational Circular Dichroism
* Emission and Circularly Polarized Luminescence (coming soon!)

The input must be in the format:

```
#NExcit TypeSpec
E1 R1
E2 R2
...
```

Where `NExcit` is an the number of excitations (no space between `#` and number),
`TypeSpec` can be `[abs,ir,cd,vcd]`, `E` must be in linear unit system (eV and
cm^{-1} are good choices, nanometer is BAD!), and `R` is in 10^{-40} cgs units only.
The `--help` option should give you a good preview of the functionality available.
Some tips:

* Gaussian and Lorentzian Broadening Value should match x-unit.
* Integration is given in spec-gen.log
** if it not equal to 1, you will need to modify the quadrature and padding.
