Here we formulate the SPIM-FCCS correlation functions within the framework described in $$ref:RIES2010:Ries, J., Z. Petrásek, A. J. Garcia-Sáez, and P. Schwille, 2010. A compre-hensive framework for fluorescence cross-correlation spectroscopy. New Journal of Physics 12:113009.$$.
There the normalized correlation function between channels g and r is written as:
  $$bmath:g_\text{gr}(\tau)=g_\infty+\frac{G_\text{gr}(\tau)}{\langle F_\text{g}(t)\rangle\cdot\langle F_\text{r}(t)\rangle}=g_\infty+\frac{\langle\delta F_g(t)\cdot\delta F_r(t+\tau)\rangle}{\langle F_\text{g}(t)\rangle\cdot\langle F_\text{r}(t)\rangle}$$
Then the non-normalized correlation function can be calculated:
  $$bmath:G_\text{gg}(\tau)&=G_\text{gg}^{A}(\tau)+G_\text{gg}^{AB}(\tau)$$  
  $$bmath:G_\text{gr}(\tau)&=G_\text{gr}^{AB}(\tau)+\kappa\cdot G_\text{gg}^{A}(\tau)+\kappa\cdot G_\text{gg}^{AB}(\tau)$$  
  $$bmath:G_\text{rr}(\tau)&=G_\text{rr}^{B}(\tau)+G_\text{rr}^{AB}(\tau)+\kappa^2\cdot G_\text{gg}^{A}(\tau)+\kappa^2\cdot G_\text{gg}^{AB}(\tau)+2\kappa\cdot G_\text{gr}^{AB}(\tau)$$  
where $$math:\kappa$$ is the crosstalk coefficient calculated from the measured fluorescence $$math:\overline{F}_\text{g/r}^\text{m}$$ and background $$math:\overline{B}_\text{g/r}^\text{m}$$ intensities:
  $$bmath:\kappa=\frac{\overline{F}_\text{r}^\text{m}-\overline{B}_\text{r}^\text{m}}{\overline{F}_\text{g}^\text{m}-\overline{B}_\text{g}^\text{m}}$$	 
The fluorescence intensities used for normalization of the CCFs are:
  $$bamth:\langle F_\text{g}(t)\rangle=\eta_\text{g}\cdot(c_\text{A}+c_\text{AB})$$
  $$bamth:\langle F_\text{r}(t)\rangle=\eta_\text{r}\cdot(c_\text{A}+c_\text{AB})+\kappa\cdot\eta_\text{g}\cdot(c_\text{A}+c_\text{AB})$$
with the channel brightnesses:
  $$bmath:\eta_\text{g}=\frac{\overline{F}_\text{g}^\text{m}-\overline{B}_\text{g}^\text{m}}{c_\text{A}+c_\text{AB}},\ \ \ \ \ \eta_\text{r}=\frac{\overline{F}_\text{r}^\text{m}-\overline{B}_\text{r}^\text{m}-\kappa\cdot(\overline{F}_\text{g}^\text{m}-\overline{B}_\text{g}^\text{m})}{c_\text{B}+c_\text{AB}}$$



