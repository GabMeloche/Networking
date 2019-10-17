using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Bonus : MonoBehaviour
{
    [SerializeField]
    int boostValue = 2;

    public int BoostValue { get { return boostValue; } }

    public delegate void OnPickedUpEventDelegate();
    public event OnPickedUpEventDelegate OnPickedUp;

    private void OnDestroy()
    {
        if (OnPickedUp != null)
            OnPickedUp();
        OnPickedUp = null;
    }
}
