<div class="system-page" id="sytem-page-id">
    <div class="header-nav">
        <ul class="header-nav-ul system-nav-ul" id="en-system-nav-ul">
            <li ><a class="navi" href="#user"><?php echo search($lpublic, "user");?></a></li>
            <li><a id="accessPointTab" class="navi" href="#accessPoint"><?php echo search($lpublic, "access_point");?></a></li>
            <li><a class="navi" href="#wirelessConfign"><?php echo search($lpublic, "wireless_configurations");?></a></li>
            <li><a class="navi system-conf-anchor" href="#systemConf"><?php echo search($lpublic, "system_configuration");?></a></li>
        </ul>
        
        <div id="user" class="system-content-div">
            <!-- Start: user Tab -->
                <?php include ("includes/tabs/user-tab-content.php");?>
            <!-- End: user Tab -->
        </div>
        <div id="accessPoint" class="system-content-div">
            <!-- Start: Recent Events Tab -->
                <?php include ("includes/tabs/accessPoint-tab-content.php");?>
            <!-- End: acessPoint Tab -->
        </div>
        <div id="wirelessConfign" class="system-content-div">
            <!-- Start: wirelessConf Tab -->
                <?php include ("includes/tabs/wirelessConf-tab-content.php");?>
            <!-- End: wirelessConf Tab -->
        </div>
        <div id="systemConf" class="system-content-div">
            <!-- Start: systemConf Tab -->
                <?php include ("includes/tabs/systemConf-tab-content.php");?>
            <!-- End: systemConf Tab -->
        </div>
      
    </div>
    <div class="system-cover"></div>
</div>