class RS_Data{
    constructor(r, s){
        this.r = r;
        this.s = s;
    }
}

class AppStroke {
    constructor(points) {
        this.reset();
        this.org_points = points;
    }

    reset(){
        this.org_points = [];

        this.org_rs     = [];

        this.rsm_points = [];
        this.app_points = [];
        this.coeffs     = [];

        this.app_rs     = [];    

        this.basis = []; 

        this.resam = 0;
        this.ord   = 0;

        this.rms_err = 0.0;
        this.max_err = 0.0;
        this.lam = 0.0;
    }

    sam() {
        return this.org_points.length/2;
      }
  
    draw(points) {
          let copy_points = this.org_points.concat(points);
          return new AppStroke(copy_points);
    }

    init_approx(resam, ord){
        console.log("init_approx: " + this.org_points.length.toString());

        if (this.org_points.length <= 2)
            return;

        this.resam = resam;
        this.ord   = ord + 1;

        this.org_rs = tracing(this.org_points);

        // initialize basis
        this.init_basis()

        // set natural parametrization
        this.reset_param();
        this.repar();

        // initial approximation
        this.approx();

        // initial error
        this.app_err(); 

        // initial approximation tracing
        this.app_rs = tracing(this.app_points);
    }

    param_app(n_itr, max_err){
        let it = 0;
        for ( ; it < n_itr; it++ )
        {
            this.repar();
            this.approx();
            this.app_err();
            this.app_rs = tracing(this.app_points);
            if (max_err > 0.0 && this.max_err < max_err)
                break;
        }
        
        return it;
    }

    init_basis() {
        let Pi = Math.PI;
        let dt0  = Pi / (2*this.resam);
        let dt1  = (Pi-2.0*dt0) / (this.resam-1);

        this.basis = new Array(this.ord*this.resam).fill(0.0);
    
        for (let i = 0; i < this.ord; i++ ) {
            let t = dt0;
            let norm = 0.0
            for (let j = 0; j < this.resam; j++, t += dt1 ){
                let b = Math.cos(i * t);
                this.basis[i*this.resam + j] = b;
                norm += b*b;
            } 

            norm = Math.sqrt(norm);
            for (let j = 0; j < this.resam; j++){
                this.basis[i*this.resam + j] /= norm;
            } 
        } // for(i)
    }

    reset_param() {
        this.app_rs = [new RS_Data(0.0, 0.0)];

	    let ds = this.org_rs[this.org_rs.length-1].r / (this.resam - 1);

	    for (let i = 1; i < this.resam; i++ ) 
            this.app_rs[i] = new RS_Data(this.app_rs[i-1].r + ds, ds);    
    }

    approx(){
        let ord   = this.ord;
        let resam = this.resam;
        this.app_points = new Array(2*resam).fill(0.0);
        for (let k = 0; k < 2; k++) {
            for (let i = 0; i < ord; i++) {
                let c  = 0.0;
                for (let j = 0;  j < resam; j++)
                    c += this.rsm_points[2*j + k] * this.basis[i*resam + j];
    
                this.coeffs[this.ord*k+i] = c;
            }
    
            for (let i = 0; i < ord; i++) {
                let c = this.coeffs[ord*k+i];
                for (let j  = 0; j < this.resam; j++)
                    this.app_points[2*j + k] += c * this.basis[i*resam + j];
            }
        }
    }

    app_err(){
        this.rms_err  = 0.0;
        this.max_err  = 0.0;
        for (let i = 0; i < this.resam; i++)
        {
            let dx = this.app_points[2*i+0] - this.rsm_points[2*i+0];  
            let dy = this.app_points[2*i+1] - this.rsm_points[2*i+1];
            let de = dx*dx + dy*dy;
     
            this.rms_err += de;
            if (de > this.max_err)
                this.max_err = de;
        }
        this.rms_err = Math.sqrt(this.rms_err / this.resam);
        this.max_err = Math.sqrt(this.max_err);
    }

    repar(){
        let n = this.sam();

        // first point
        this.rsm_points = [this.org_points[0], this.org_points[1]];

	    //m_pAppRS[0].idx = 0;
	    //m_pAppRS[0].alf = 0.0f;

	    this.lam  = this.org_rs[n-1].r / this.app_rs[this.resam-1].r;

        let j = 1;
	    for (let i = 1; i < (this.resam - 1); i++) {
		    let curr = this.lam * this.app_rs[i].r;

		    while (curr  > this.org_rs[j].r + 0.001) { 
			    //m_pOrgRS[j].idx = i;
			    j++;
		 
			    if (j == n) {
				    for ( ; i < this.resam; i++) {
						this.rsm_points[2*i+0] = this.org_points[2*(n-1)+0] / this.lam;
                        this.rsm_points[2*i+1] = this.org_points[2*(n-1)+1] / this.lam;
				    }
				    break;
			    }	 // if( j==n )
		    }	// while

            if (j == n)
                break;

            let alfa = 1.0;
		    if (this.org_rs[j].s > 0.0)
			    alfa = 1.0 - (this.org_rs[j].r - curr) / this.org_rs[j].s;

		    for (let k = 0; k < 2; k++){
			    let x2 = this.org_points[2*(j  )+k];
			    let x1 = this.org_points[2*(j-1)+k];
			    let dx = x2 - x1;
			    this.rsm_points[2*i+k] = x1 + dx*alfa;
		    }

		    //m_pAppRS[i].idx = j-1;
		    //m_pAppRS[i].alf = Alfa;
        } // for(i)

        // last point
        this.rsm_points.push(this.org_points[2*(n-1) + 0]);
        this.rsm_points.push(this.org_points[2*(n-1) + 1]);
    //END:
    // m_pAppRS[m_ReSam-1].idx = m_Sam-1;
    // m_pAppRS[m_ReSam-1].alf = 1.0f;
    }

}


function tracing(points){
    let rs = [ new RS_Data(0.0, 0.0) ];
    
    if (points.length > 2){
        for(let i = 2; i < points.length; i+=2) {
            let dx = points[i+0] - points[i-2];
            let dy = points[i+1] - points[i-1];
            let ds = Math.sqrt(dx*dx + dy*dy); 
            rs.push(new RS_Data(rs[rs.length-1].r + ds, ds));
        }
    }
    return rs;
}

/*
let D = 100.0
stroke = new AppStroke( 
    [  D,   D, 
     2*D,   D,
     2*D, 2*D,
       D, 2*D,
       D,   D]);

stroke.init_approx(32, 11);
stroke.param_app(10, -1.0);
*/
