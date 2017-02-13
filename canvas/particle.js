
// Developed by nupamore
// 2014-03-08

var timeElapsed;
var particle={
	pixelMAX:100,
	home:{},
	context:{},
	list:[],
	pixel:[],
	pixels:[],
	pixelAddress:[],
	create:function(){},
	start:function(){}
};

var P=function(){return new particle.create(); };



particle.create=function(){
	var p=particle,

		id=0,
		pixelStatus=[],
		pixelActivity=[],
		live=true,
		style={amount:p.pixelMAX,
			fps:60,
			scope:'pix.x>0&&pix.y>0&&pix.x<1200&&pix.y<850',
			life:10000
		},
		init={
			x:0,
			y:0,
			size:0,
			shape:'.',
			color:'rgb(0,0,0)',
			opacity:100
		},
		animate={
			mx:0,
			my:0,
			ax:0,
			ay:0,
			msize:0
		},
		analyze=function(time,t){
			setTimeout(function(){
				var max=p.pixelMAX;
				var amount=style.amount;
				for(var i=0, num=0;(i<max)&&(num<amount);i++){
					if(p.pixelAddress[i]==null){
						p.pixelAddress[i]=id;
						p.pixels[i].size=eval(init.size);
						pixelActivity[i]=pixelActivityFactory(i);
						pixelActivityFactory(i);
						pixelActivity[i]();
						num++;
					}
				}
				drawCanvas();
			},time);
		},
		pixelActivityFactory=function(i){
			var pix;
			var red, green, blue;
			pixelStatus[i]={
				id:i,
				frame:0,
				x:eval(init.x),
				y:eval(init.y),
				mx:eval(animate.mx),
				my:eval(animate.my),
				ax:eval(animate.ax),
				ay:eval(animate.ay),
				size:eval(init.size)
			};
			pix=pixelStatus[i];

			red = 255;
			green = 255;
			blue = Math.round(Math.random()*100+150);
			pixelStatus[i].color = 'rgb('+red+','+green+','+blue+')';

			return function f(){
				pixelStatus[i].mx+=pixelStatus[i].ax;
				pixelStatus[i].my+=pixelStatus[i].ay;
				pixelStatus[i].x+=pixelStatus[i].mx;
				pixelStatus[i].y+=pixelStatus[i].my;
				pixelStatus[i].frame++;

				if(eval(style.scope)&&pixelStatus[i].frame<style.life&&live){
					setTimeout(f,(60/style.fps)*10);

				}else{
					p.pixels[i].display='none';
					p.pixelAddress[i]=null;
				}
			};
		},
		drawCanvas=function(){
			var startTime = new Date().getTime();

			var max=p.pixelMAX;
			var amount=style.amount;

			p.context.globalAlpha = 0.1;
			p.context.fillStyle = 'rgb(0,0,0)';
			p.context.fillRect(0, 0, p.home.width, p.home.height);

			for(var i=0, num=0;(i<max)&&(num<amount);i++){
				p.context.globalAlpha = 1;
				p.context.fillStyle = pixelStatus[i].color;
				p.context.fillRect(pixelStatus[i].x, top=pixelStatus[i].y, pixelStatus[i].size, pixelStatus[i].size);
				num++;
			}

			timeElapsed2 = new Date().getTime() - startTime;
      requestAnimationFrame(drawCanvas);
		};
	this.getID=function(){
		return	id;
	};
	this.style=function(s){
		for (var prop in s){
			if(style[prop]!=null){
				style[prop]=s[prop];
			}

		}
		return this;
	};
	this.init=function(i){
		for(var prop in i){
			if(init[prop]!=null){
				init[prop]=i[prop];
			}
		}
		return this;
	};
	this.animate=function(a){
		for(var prop in a){
			if(animate[prop]!=null){
				animate[prop]=a[prop];
			}
		}
		return this;
	};
	this.reAnimate=function(a){
		var startTime = new Date().getTime();

		var id=this.getID(),max=style.amount;
		for(var i=0;i<max;i++){
			if(p.pixelAddress[i]==id){
				var pix=pixelStatus[i];
				for(var prop in a){
					if(pix[prop]!=null){
						pix[prop]=eval(a[prop]);
					}
				}
			}
		}

		timeElapsed = new Date().getTime() - startTime;

		return this;
	};
	this.run=function(time){
		analyze(time,this);
		return this;
	};
	this.dead=function(time){
		setTimeout(function(){live=false;},time);
	};
	id=p.list.length;
	p.list[id]=this;
	return this;
};

particle.start=function(max){
	var p=particle;
	if(max!=null){
		p.pixelMAX=max;
	}
	p.home=document.getElementById('particleHome');
	if(p.home==null){
		document.body.innerHTML="<canvas id='particleHome' width='"+window.screen.availWidth+"' height='"+window.screen.availHeight+"'>Sorry, Your browser doesn't support HTML5 CANVAS</canvas>"+document.body.innerHTML;
		p.home=document.getElementById('particleHome');
	}
	p.context=p.home.getContext("2d");
	for(var i=0;i<p.pixelMAX;i++){
		p.pixelAddress[i]=null;
		p.pixel[i]=null;
		p.pixels[i] = {
			left:0,
			top:0,
			fontSize:10,
			color:"rgb(0,0,0)",
			size:10,
			display:true
		};
	}
};
