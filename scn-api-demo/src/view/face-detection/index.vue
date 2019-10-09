<style lang="less">
    @import "./face-detection.less";
</style>
<template>
        <div class="face-detection-page">
            <div class="main" style="min-height:460px;" >
                <Row :gutter="16">
                    <Col span="12">
                        <div>
                            <H3>Face Detection</H3>
                            <p style="line-height:28px;">Invoke face detection api on blockcloud function delivery network which is deploy at edge server.</p>
                            <p>Upload image through the upload button on the right.  Supports types: JPG, JPEG, PNG; Less than 10 MB</p>
                        </div>
                    </Col>
                    <Col span="6" offset="6">
                        <Upload  
                            type="drag" 
                            action="/" 
                            name="file" 
                            accept='image/jpg,image/jpeg,image/png'
                            :max-size="2048"
                            :before-upload="uploadImg">
                            <div style="padding: 8px 0">
                                <Icon type="ios-cloud-upload" size="24" style="color: #3399ff"></Icon>
                                <p>Click or drag picture here to upload</p>
                            </div>
                        </Upload>
                    </Col>
                </Row>    
               
                <Divider style="margin:8px 0 24px 0;"/>
                <div class="tool-block">
                    <Alert style="padding-right:16px;">
                        <span>
                            Mask Border Color: <ColorPicker v-model="strokeColor"  @on-change='changeMaskStyle' size="small" />
                        </span>
                        <Divider type="vertical" />
                        <span>
                            Mask Background Color: <ColorPicker v-model="maskColor" alpha @on-change='changeMaskStyle' size="small" />
                        </span>
                        <Divider type="vertical" />
                        <span>
                            Feature Points Color : <ColorPicker v-model="featurePointsColor" alpha @on-change='changeMaskStyle' size="small" />
                        </span> 
                        <Divider type="vertical" />   
                        <span>
                            Feature Points :
                            <i-switch size="large" @on-change='changeMaskStyle' v-model="showFeaturePoints">
                                <span slot="open">Show</span>
                                <span slot="close">Hide</span>
                            </i-switch>
                        </span>  
                        <Button size="small" type="default" @click='resetMaskStyle' :disabled="!maskStyleChanged" style="margin-left:16px;">
                                <Icon type="ios-refresh"  size="20" />
                                Reset Style
                        </Button>
                    </Alert>  
                </div>


                <Row :gutter="16">
                    <Col span="12">
                        <Card dis-hover>
                            <p slot="title">Origin</p>
                            <div class="origin-picture-container" id="origin-picture-container" >
                                <img  :src="originPic" v-if="originPic"  id='originPicDom'  />
                            </div>
                        </Card>
                    </Col>
                    <Col span="12" class="canvas-container" >
                        <Card dis-hover>
                            <p slot="title">Face Detection Result</p>
                            <span slot="extra"  v-show='completed'> <b>{{facesCount}}</b>  Face(s)  </span>
                            <div class="canvas-card-body" >
                                <div class="canvas-tips" v-show='!completed' v-if='needLoading'>
                                    <Spin fix>
                                        <Icon type="ios-loading" size=18 class="spin-icon-load"></Icon>
                                        <div>{{progressText}}</div>
                                    </Spin>
                                </div>
                                <canvas 
                                    id="resultCanvas"
                                    :width='canvasWidth'
                                    :height='canvasHeight'
                                     v-if="originPic" 
                                ></canvas>
                            </div>
                        </Card>   
                    </Col>
                </Row>
        </div>
    </div>
</template>

<script>
    import { uploadBase64 } from '@/api/fdn';
    const originBase64 = require('../../assets/images/logo2x.png');
    const maskDefaultStyle = {
                                strokeColor:'#10F924',
                                maskColor:'rgba(240,250,255,0.5)',
                                featurePointsColor:'#00f'
                            }
    const  progressTextList=['Uploading... ','Upload completed, got the result and start drawing result'];
                        
    export default {
        name: 'FaceDetection',
        data () {
            return {
                maskStyleChanged:false,
                originPic:null,
                strokeColor:maskDefaultStyle.strokeColor,
                maskColor:maskDefaultStyle.maskColor,
                featurePointsColor:maskDefaultStyle.featurePointsColor,
                canvasWidth:0,
                canvasHeight:0,
                maskList:[],
                facesCount:0,
                showFeaturePoints:false,
                needLoading:false,
                completed:false,
                progressText:progressTextList[0]
            }
        },
        created(){
        },
        beforeMount(){
        },
        mounted(){
            
        },
        watch: {
            scale(){
                this.scaling = 'scale('+this.scale+')';
            }
        },
        computed: {
            
        },
        methods: {
            showGloabLoading(){
                this.$Spin.show({
                    render: (h) => {
                        return h('div', [
                            h('Icon', {
                                'class': 'spin-icon-load',
                                props: {
                                    type: 'ios-loading',
                                    size: 18
                                }
                            }),
                            h('div', 'Loading')
                        ])
                    }
                });
            },
            hideGloabLoading(){
                this.$Spin.hide();
            },
            uploadImg(file){
                let _this = this;
                let imgZoom = 1;
                let fileSize = file.size;
                let fileType = file.type;
                if(fileSize > 1024000 && fileSize<10240000 ){
                    imgZoom = Math.floor(fileSize/1024000)*2;
                }else if(fileSize > 10240000){
                    alert('The image too large !');
                    return;
                }
                this.completed = false;
                this.needLoading = true;
                var originConWidth = document.defaultView.getComputedStyle(document.getElementById('origin-picture-container')).width;
                originConWidth = parseInt(originConWidth);
                //console.log('=======imgZoom',imgZoom);
                var reader = new FileReader();
                reader.readAsDataURL(file);
                let image = new Image();
                reader.onload = function(e) {
                    var base64 = e.target.result;
                    //_this.originPic = base64; 
                    file.src = base64;
                    image.src = base64;
                    image.onload = function(){
                        let that = this;
                        //console.log(11111, image.width,image.height,originConWidth);
                        let width = image.width;
                        let height = image.height;
                        file.width = width;
                        file.height = height;
                        //console.log('=======',file);
                        // 默认按比例压缩
                        var w = file.width/imgZoom;
                        var h = file.height/imgZoom;
                        if(originConWidth<w){
                            w = originConWidth;
                            h =  (originConWidth/file.width)*file.height;
                        }
                         _this.canvasWidth = w;
                        _this.canvasHeight = h;
                        var quality = 0.9;  // 默认图片质量为0.7
                        //生成canvas
                        var canvas = document.createElement('canvas');
                        var ctx = canvas.getContext('2d');
                        // 创建属性节点
                        var anw = document.createAttribute("width");
                        anw.nodeValue = w;
                        var anh = document.createAttribute("height");
                        anh.nodeValue = h;
                        canvas.setAttributeNode(anw);
                        canvas.setAttributeNode(anh);
                        ctx.drawImage(image, 0, 0, w, h);
                        //console.log('ctx',ctx);
                        // 图像质量
                        // quality值越小，所绘制出的图像越模糊
                        var canvas2base64 = canvas.toDataURL(fileType, quality);
                        _this.originPic = canvas2base64  //页面上显示所选择的图片
                        //console.log('canvas2base64',canvas2base64);
                        // 回调函数返回base64的值
                        let base64Str = canvas2base64.split(',')[1];
                        _this.progressText = progressTextList[0];
                        _this.maskList = [];
                        // 正在上传图片
                        uploadBase64({"image":base64Str}).then(res=>{
                            //console.log('========res===== 1')
                            //console.log(res.response)
                            //console.log('========res===== 2')
                            // 接收到服务器返回的计算结果,正在绘制图片
                            _this.progressText = progressTextList[1];
                            //"faces": [[256, 86, 362, 238]],
                            if(res.data && res.data.faces){
                                let faces = res.data.faces;                                
                                let featurePoints = res.data.feature_points;
                                let facesCount = faces.length;
                                _this.facesCount = facesCount;
                                faces.forEach((face,index) => {
                                    // 01    21
                                    // 03    23
                                    let lt = [face[0],face[1]];
                                    let lb = [face[0],face[3]];
                                    let rt = [face[2],face[1]];
                                    let rb = [face[2],face[3]];
                                    //that.coordinates.push([lt,rt,rb,lb]);
                                    //(x,y,width,height)
                                    let maskPoint_x = face[0];
                                    let maskPoint_y = face[1];
                                    let maskPoint_width = face[2]-face[0];
                                    let maskPoint_height = face[3]-face[1];
                                    let mask = {
                                        point:[lt,rt,rb,lb],
                                        canvas:[maskPoint_x,maskPoint_y,maskPoint_width,maskPoint_height],
                                        featurePoints:featurePoints[index]
                                    }
                                    _this.maskList.push(mask);
                                });
                                //console.log(that.maskList);
                                setTimeout(()=>{
                                    _this.drawResult()                  
                                },1000) 
                            }else{
                                let msg = 'error';
                                if(res.response){
                                    if(res.response.data){
                                        if(res.response.data.error){
                                            msg = res.response.data.error;
                                        }
                                    }
                                }
                                alert(msg);
                            }

                        },err=>{
                            //console.log(err);
                        }).catch((error) => {
                            //console.log('========catch===== 1')
                            //console.log(error)
                            //console.log('========catch===== 2')
                            alert('error')
                        })
                    };
                }
                return false;  
            },            
            drawResult(){
                var reg = /^\s*data:([a-z]+\/[a-z0-9-+.]+(;[a-z-]+=[a-z0-9-]+)?)?(;base64)?,([a-z0-9!$&',()*+;=\-._~:@\/?%\s]*?)\s*$/i;
                if(!reg.test(this.originPic)){
                    this.throwMessage('error','invalid picture data');
                    return;
                }
                var dom=document.getElementById("resultCanvas");
                var ctx=dom.getContext("2d");
                var img=new Image()
                img.src=this.originPic;
                ctx.drawImage(img,0,0);
                ctx.beginPath();
                ctx.strokeStyle=this.strokeColor;
                ctx.fillStyle = this.maskColor;
                ctx.lineWidth=1;
                this.maskList.forEach(mask=>{
                    ctx.strokeRect(...mask['canvas']);  
                    ctx.fillRect(...mask['canvas']);  
                    if(this.showFeaturePoints){
                        let featurePoints = mask['featurePoints'];
                        featurePoints.forEach(points=>{
                            ctx.strokeStyle = this.featurePointsColor;
                            let crossSize = 4;
                            ctx.moveTo(points[0],points[1]-crossSize);
                            ctx.lineTo(points[0],points[1]+crossSize);
                            ctx.moveTo(points[0]-crossSize,points[1]);
                            ctx.lineTo(points[0]+crossSize,points[1]);
                        })
                    }
                })
                ctx.stroke();
                this.completed=true;
            },
            changeMaskStyle(){
                if(
                    this.strokeColor == maskDefaultStyle.strokeColor 
                    && this.maskColor == maskDefaultStyle.maskColor
                    && this.featurePointsColor == maskDefaultStyle.featurePointsColor
                    && this.showFeaturePoints==false
                ){
                    this.maskStyleChanged = false;
                }else{
                    this.maskStyleChanged = true;
                }
               
                if(this.originPic){
                    this.drawResult();
                }
            },
            resetMaskStyle(){
                this.strokeColor = maskDefaultStyle.strokeColor;
                this.maskColor = maskDefaultStyle.maskColor;
                this.featurePointsColor = maskDefaultStyle.featurePointsColor;
                this.showFeaturePoints = false;
                this.maskStyleChanged = false;
                if(this.originPic){
                    this.drawResult();
                }
            },
            toggleFeaturePoints(){
                //console.log(this.showFeaturePoints);
                //this.showFeaturePoints = !this.showFeaturePoints;
                if(this.originPic){
                    this.drawResult();
                }
            },
            throwMessage(msgType,msg){
                if(msgType=='success'){
                    this.$Message.success(msg);
                }else if(msgType=='warning'){
                    this.$Message.warning(msg);
                }else if(msgType=='error'){
                    this.$Message.error(msg);
                }else{
                    this.$Message.info(msg);
                }
            }
        },
        mounted () {

        }
    }
</script>
