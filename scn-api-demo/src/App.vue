<template>
  <div id="app" class="layout">
        <Layout>
            <Header>
                <Menu mode="horizontal" theme="dark" active-name="1">
                    <div class="layout-logo">
                      <img src="./assets/images/logo.png" />
                    </div>
                    <div class="layout-nav">
                    </div>
                </Menu>
            </Header>
            <Content :style="{padding: '0 50px',marginTop:'16px'}">
                <Card dis-hover>
                    <div style="min-height: 200px;">
                       <FaceDetection></FaceDetection>
                    </div>
                </Card>
            </Content>
            <Footer class="layout-footer-center"> &copy; Blockcloud</Footer>
        </Layout>
    </div>  
</template>

<script>

import FaceDetection from './view/face-detection'

export default {
  name: 'app',
  components: {
    FaceDetection  
    }
}
</script>

<style scoped>

.layout{
        border: 1px solid #d7dde4;
        background: #f5f7f9;
        position: relative;
        border-radius: 4px;
        overflow: hidden;
    }
    .layout-logo{
        width: 160px;
        border-radius: 3px;
        float: left;
    }
    .layout-logo img{
      width: 100%;
      margin-top: 20px;
    }
    .layout-nav{
        width: 420px;
        margin: 0 auto;
        margin-right: 20px;
    }
    .layout-footer-center{
        text-align: center;
    }
</style>
