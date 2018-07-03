import * as $ from 'jquery';
import "jquery-ui/ui/widgets/tabs";
import "jquery-ui/ui/widgets/dialog";

export class AdvancedSettingsOverlay {
    public constructor() {
        $("#advanced-settings-overlay").dialog({
            modal: true,
            width: 600,
            height: 400,
            autoOpen: false,
        }).parent().draggable({
            containment: "#container"
        });
        $("#advanced-settings-overlay-tabs").tabs({
            heightStyle: "fill"
        });
    }
}