package mobi.designmyapp.arpigl.listener;

public interface PoiSelectionListener {

    /**
     * Called when a poi is selected.
     * @param sid the poi sid
     */
    void onPoiSelected(String sid);

    /**
     * Called when a poi is deselected.
     * @param sid the poi sid
     */
    void onPoiDeselected(String sid);
}
